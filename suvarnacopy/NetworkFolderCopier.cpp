#include "NetworkFolderCopier.h"
#include <QCoreApplication>
#include <QRegularExpression>

#ifdef Q_OS_WIN
#include <windows.h>
#include <lm.h>
#include <winnetwk.h>
#pragma comment(lib, "mpr.lib")
#pragma comment(lib, "netapi32.lib")
#endif

NetworkFolderCopier::NetworkFolderCopier(QObject *parent)
    : QObject(parent)
    , m_verbose(false)
    , m_cancelled(false)
{
    m_totalTimer.start();
}

NetworkFolderCopier::~NetworkFolderCopier()
{
}

void NetworkFolderCopier::startCopy(const QString &sourcePath, const QString &destPath,
                                    const QString &username, const QString &password,
                                    const QString &domain)
{
    m_currentJob.sourcePath = normalizePath(sourcePath);
    m_currentJob.destPath = normalizePath(destPath);
    m_currentJob.username = username;
    m_currentJob.password = password;
    m_currentJob.domain = domain;
    m_currentJob.useAuthentication = !username.isEmpty() || !password.isEmpty();

    m_stats = CopyStats();
    m_cancelled = false;

    log(QString("Starting folder copy operation..."), true);
    log(QString("Source: %1").arg(m_currentJob.sourcePath), true);
    log(QString("Destination: %1").arg(m_currentJob.destPath), true);

    if (m_currentJob.useAuthentication) {
        log(QString("Using authentication for user: %1%2%3")
            .arg(m_currentJob.domain.isEmpty() ? "" : m_currentJob.domain + "\\")
            .arg(m_currentJob.username)
            .arg(m_currentJob.password.isEmpty() ? "" : " (password provided)"), true);
    }

    // Check if paths are accessible
    if (!checkNetworkPath(m_currentJob.sourcePath)) {
        if (isNetworkPath(m_currentJob.sourcePath)) {
            emit authenticationRequired(m_currentJob.sourcePath);
            QTimer::singleShot(100, this, &NetworkFolderCopier::retryWithAuthentication);
            return;
        } else {
            emit error(QString("Source path is not accessible: %1").arg(m_currentJob.sourcePath));
            return;
        }
    }

    // Perform the copy
    bool success = copyDirectoryContents(m_currentJob.sourcePath, m_currentJob.destPath);

    if (m_cancelled) {
        log("Copy operation cancelled by user", true);
        emit finished();
        return;
    }

    if (success) {
        qint64 elapsed = m_totalTimer.elapsed();
        log("\n" + QString("=").repeated(60), true);
        log("COPY OPERATION COMPLETED SUCCESSFULLY", true);
        log(QString("=").repeated(60), true);
        log(QString("Total time: %1").arg(formatTime(elapsed)), true);
        log(QString("Directories created: %1/%2").arg(m_stats.createdDirs).arg(m_stats.totalDirs), true);
        log(QString("Files copied: %1/%2").arg(m_stats.copiedFiles).arg(m_stats.totalFiles), true);
        log(QString("Data transferred: %1/%2")
            .arg(formatSize(m_stats.copiedBytes))
            .arg(formatSize(m_stats.totalBytes)), true);
        log(QString("Average speed: %1/s")
            .arg(formatSize(elapsed > 0 ? (m_stats.copiedBytes * 1000 / elapsed) : 0)), true);
        if (m_stats.failedItems > 0) {
            log(QString("Failed items: %1").arg(m_stats.failedItems), true);
        }
        log(QString("=").repeated(60), true);

        emit finished();
    } else if (!m_cancelled) {
        emit error("Failed to copy folder contents");
    }
}

void NetworkFolderCopier::cancel()
{
    QMutexLocker locker(&m_mutex);
    m_cancelled = true;
}

void NetworkFolderCopier::handleAuthentication(const QString &networkPath)
{
    log(QString("Authentication required for: %1").arg(networkPath));

    if (m_currentJob.useAuthentication) {
        if (authenticateNetworkPath(networkPath)) {
            log("Authentication successful");
        } else {
            emit error("Authentication failed");
        }
    } else {
        emit error("Network path requires authentication but no credentials provided");
    }
}

void NetworkFolderCopier::retryWithAuthentication()
{
    if (authenticateNetworkPath(m_currentJob.sourcePath)) {
        // Retry the copy
        startCopy(m_currentJob.sourcePath, m_currentJob.destPath,
                 m_currentJob.username, m_currentJob.password,
                 m_currentJob.domain);
    } else {
        emit error("Failed to authenticate network path");
    }
}

bool NetworkFolderCopier::copyDirectoryContents(const QString &sourcePath, const QString &destPath)
{
    QDir sourceDir(sourcePath);

    if (!sourceDir.exists()) {
        emit error(QString("Source directory does not exist: %1").arg(sourcePath));
        return false;
    }

    // Create destination directory if it doesn't exist
    if (!createDirectory(destPath)) {
        return false;
    }

    // Get all files and directories
    QFileInfoList entries = sourceDir.entryInfoList(QDir::Files | QDir::Dirs | QDir::NoDotAndDotDot);

    // First pass: count total items and bytes
    for (const QFileInfo &entry : entries) {
        if (m_cancelled) return false;

        if (entry.isDir()) {
            m_stats.totalDirs++;
        } else {
            m_stats.totalFiles++;
            m_stats.totalBytes += entry.size();
        }
    }

    log(QString("\nFound %1 directories and %2 files (%3 total)")
        .arg(m_stats.totalDirs)
        .arg(m_stats.totalFiles)
        .arg(formatSize(m_stats.totalBytes)), true);

    // Second pass: copy contents
    for (const QFileInfo &entry : entries) {
        if (m_cancelled) return false;

        QString sourceEntryPath = entry.absoluteFilePath();
        QString destEntryPath = destPath + "/" + entry.fileName();

        if (entry.isDir()) {
            log(QString("\n[Directory] %1").arg(entry.fileName()));

            if (!copyDirectoryContents(sourceEntryPath, destEntryPath)) {
                m_stats.failedItems++;
            } else {
                m_stats.createdDirs++;
            }
        } else {
            if (copyFile(sourceEntryPath, destEntryPath)) {
                m_stats.copiedFiles++;
                m_stats.copiedBytes += entry.size();
            } else {
                m_stats.failedItems++;
            }
            updateProgress();
        }
    }

    return m_stats.failedItems == 0;
}

bool NetworkFolderCopier::copyFile(const QString &sourcePath, const QString &destPath)
{
    QFile sourceFile(sourcePath);
    QFileInfo sourceInfo(sourcePath);

    emit fileCopyStarted(sourceInfo.fileName(), sourceInfo.size());
    m_fileTimer.start();

    log(QString("  Copying: %1 (%2)")
        .arg(sourceInfo.fileName())
        .arg(formatSize(sourceInfo.size())));

    // Check if destination file exists and handle it
    if (QFile::exists(destPath)) {
        if (!QFile::remove(destPath)) {
            log(QString("  Failed to remove existing file: %1").arg(destPath));
            emit fileCopyFinished(sourceInfo.fileName(), false);
            return false;
        }
    }

    // Perform the copy
    if (!QFile::copy(sourcePath, destPath)) {
        log(QString("  Failed to copy file: %1").arg(sourcePath));
        emit fileCopyFinished(sourceInfo.fileName(), false);
        return false;
    }

    // Preserve file permissions if possible
    QFile destFile(destPath);
    destFile.setPermissions(sourceFile.permissions());

    qint64 elapsed = m_fileTimer.elapsed();
    log(QString("  Completed in %1 (%2/s)")
        .arg(formatTime(elapsed))
        .arg(formatSize(elapsed > 0 ? (sourceInfo.size() * 1000 / elapsed) : 0)));

    emit fileCopyFinished(sourceInfo.fileName(), true);
    return true;
}

bool NetworkFolderCopier::createDirectory(const QString &path)
{
    QDir dir(path);
    if (!dir.exists()) {
        log(QString("Creating directory: %1").arg(QFileInfo(path).fileName()));

        if (!dir.mkpath(".")) {
            emit error(QString("Failed to create directory: %1").arg(path));
            return false;
        }

        emit directoryCreated(path);
    }
    return true;
}

bool NetworkFolderCopier::authenticateNetworkPath(const QString &path)
{
#ifdef Q_OS_WIN
    QString server = extractServerFromPath(path);
    if (server.isEmpty()) {
        log("Failed to extract server from path: " + path);
        return false;
    }

    // Convert QString to wide string and store in a buffer
    std::wstring remoteName = server.toStdWString();
    std::wstring userName;
    std::wstring userPassword = m_currentJob.password.toStdWString();

    // Build username with domain if provided
    if (!m_currentJob.domain.isEmpty()) {
        userName = m_currentJob.domain.toStdWString() + L"\\" + m_currentJob.username.toStdWString();
    } else {
        userName = m_currentJob.username.toStdWString();
    }

    // Setup NETRESOURCE structure
    NETRESOURCEW nr = {0};
    nr.dwType = RESOURCETYPE_DISK;
    nr.lpRemoteName = const_cast<LPWSTR>(remoteName.c_str());  // Safe cast as the structure doesn't modify the string

    // Make the network connection
    DWORD result = WNetAddConnection2W(
        &nr,
        userPassword.empty() ? nullptr : userPassword.c_str(),
        userName.empty() ? nullptr : userName.c_str(),
        0
    );

    if (result == NO_ERROR) {
        log(QString("Successfully connected to: %1").arg(server));
        return true;
    } else {
        // Get error message
        wchar_t errorMsg[256] = {0};
        DWORD errorChars = FormatMessageW(
            FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
            nullptr,
            result,
            MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
            errorMsg,
            sizeof(errorMsg) / sizeof(wchar_t),
            nullptr
        );

        QString errorString = errorChars > 0 ?
            QString::fromWCharArray(errorMsg).trimmed() :
            QString("Unknown error code: %1").arg(result);

        log(QString("Failed to connect to %1. Error: %2")
            .arg(server).arg(errorString));
        return false;
    }
#else
    // For Linux/Mac, try using smbclient or mount command
    QProcess process;
    QStringList arguments;

    QString server = extractServerFromPath(path);
    if (server.isEmpty()) {
        log("Failed to extract server from path: " + path);
        return false;
    }

    // Remove backslashes and convert to SMB URL format
    QString smbPath = server.replace("\\", "/");
    if (!smbPath.startsWith("//")) {
        smbPath = "//" + smbPath;
    }

    if (!m_currentJob.username.isEmpty()) {
        if (!m_currentJob.domain.isEmpty()) {
            arguments << "-U" << (m_currentJob.domain + "\\" + m_currentJob.username);
        } else {
            arguments << "-U" << m_currentJob.username;
        }

        if (!m_currentJob.password.isEmpty()) {
            // For smbclient, password can be passed via environment variable or command line
            // Using command line for simplicity, though less secure
            arguments << m_currentJob.password;
        }
    }

    arguments << smbPath;

    log(QString("Attempting SMB connection to: %1").arg(smbPath));

    // Check if smbclient is available
    QString smbClientPath = QProcessEnvironment::systemEnvironment().value("SMB_CLIENT", "smbclient");

    process.start(smbClientPath, arguments);

    if (!process.waitForStarted(5000)) {
        log("Failed to start smbclient. Is it installed?");
        return false;
    }

    // Wait for authentication to complete or fail
    if (!process.waitForFinished(30000)) {
        process.kill();
        log("smbclient timed out");
        return false;
    }

    bool success = (process.exitCode() == 0);

    if (success) {
        log(QString("Successfully authenticated with: %1").arg(server));
    } else {
        QString errorOutput = QString::fromLocal8Bit(process.readAllStandardError());
        log(QString("Authentication failed: %1").arg(errorOutput));
    }

    return success;
#endif
}

bool NetworkFolderCopier::checkNetworkPath(const QString &path)
{
    if (!isNetworkPath(path)) {
        return QDir(path).exists();
    }

#ifdef Q_OS_WIN
    // Try to access the network path
    QDir dir(path);
    if (dir.exists()) {
        return true;
    }

    // If it doesn't exist, try to authenticate
    if (m_currentJob.useAuthentication) {
        return authenticateNetworkPath(path);
    }
#endif

    return false;
}

QString NetworkFolderCopier::normalizePath(const QString &path)
{
    QString normalized = path;

    // Remove any surrounding quotes
    if (normalized.startsWith('"') && normalized.endsWith('"')) {
        normalized = normalized.mid(1, normalized.length() - 2);
    }

#ifdef Q_OS_WIN
    // Convert forward slashes to backslashes for Windows
    normalized.replace("/", "\\");

    // Handle UNC paths
    if (normalized.startsWith("\\\\?\\")) {
        // Already in extended-length path format
        return normalized;
    }

    // Ensure UNC paths have double backslashes at the beginning
    if (normalized.startsWith("\\\\")) {
        // Valid UNC path, ensure no trailing backslash
        while (normalized.endsWith("\\") && normalized.length() > 3) {
            normalized.chop(1);
        }
    } else if (normalized.startsWith("\\") && !normalized.startsWith("\\\\")) {
        // Single backslash at start - convert to UNC
        normalized = "\\\\" + normalized.mid(1);
    }

    // Convert to extended-length path for very long paths
    if (normalized.length() > 260 && !normalized.startsWith("\\\\?\\")) {
        if (normalized.startsWith("\\\\")) {
            normalized = "\\\\?\\UNC\\" + normalized.mid(2);
        } else {
            normalized = "\\\\?\\" + normalized;
        }
    }
#else
    // For Unix-like systems, convert backslashes to forward slashes
    normalized.replace("\\", "/");

    // Handle network paths
    if (normalized.startsWith("//") || normalized.startsWith("\\\\")) {
        // Convert to SMB URL format if needed
        if (!normalized.startsWith("smb:")) {
            normalized = "smb:" + normalized;
        }
    }
#endif

    return normalized;
}

bool NetworkFolderCopier::isNetworkPath(const QString &path)
{
    QString normalized = path;

#ifdef Q_OS_WIN
    // Windows network paths
    return normalized.startsWith("\\\\") ||
           normalized.startsWith("//") ||
           normalized.contains("\\\\?\\UNC\\");
#else
    // Unix network paths
    return normalized.startsWith("//") ||
           normalized.startsWith("smb://") ||
           normalized.startsWith("\\\\");
#endif
}
QString NetworkFolderCopier::extractServerFromPath(const QString &path)
{
    QString normalized = path;

    // Handle different path formats
    if (normalized.startsWith("\\\\")) {
        // Windows UNC path: \\server\share\path
        QStringList parts = normalized.split("\\", Qt::SkipEmptyParts);
        if (parts.size() >= 2) {
            return "\\\\" + parts[0] + "\\" + parts[1];
        }
    } else if (normalized.startsWith("//")) {
        // Unix-style network path: //server/share/path
        QStringList parts = normalized.split("/", Qt::SkipEmptyParts);
        if (parts.size() >= 2) {
            return "//" + parts[0] + "/" + parts[1];
        }
    }

    return QString();
}
QString NetworkFolderCopier::formatSize(qint64 bytes)
{
    const char* suffixes[] = {"B", "KB", "MB", "GB", "TB"};
    int suffixIndex = 0;
    double size = bytes;

    while (size >= 1024 && suffixIndex < 4) {
        size /= 1024;
        suffixIndex++;
    }

    return QString("%1 %2").arg(size, 0, 'f', suffixIndex > 0 ? 1 : 0).arg(suffixes[suffixIndex]);
}

QString NetworkFolderCopier::formatTime(qint64 ms)
{
    int seconds = ms / 1000;
    int minutes = seconds / 60;
    int hours = minutes / 60;

    seconds %= 60;
    minutes %= 60;

    if (hours > 0) {
        return QString("%1h %2m %3s").arg(hours).arg(minutes).arg(seconds);
    } else if (minutes > 0) {
        return QString("%1m %2s").arg(minutes).arg(seconds);
    } else {
        return QString("%1.%2s").arg(seconds).arg((ms % 1000) / 100);
    }
}

void NetworkFolderCopier::log(const QString &message, bool always)
{
    if (always || m_verbose) {
        qInfo().noquote() << message;
        emit progress(message);
    }
}

void NetworkFolderCopier::updateProgress()
{
    if (m_stats.totalFiles > 0) {
        int percent = (m_stats.copiedFiles * 100) / m_stats.totalFiles;
        qint64 elapsed = m_totalTimer.elapsed();

        log(QString("\rProgress: %1% (%2/%3 files, %4/%5) - Elapsed: %6 - Speed: %7/s    ")
            .arg(percent)
            .arg(m_stats.copiedFiles)
            .arg(m_stats.totalFiles)
            .arg(formatSize(m_stats.copiedBytes))
            .arg(formatSize(m_stats.totalBytes))
            .arg(formatTime(elapsed))
            .arg(formatSize(elapsed > 0 ? (m_stats.copiedBytes * 1000 / elapsed) : 0)), false);
    }
}
