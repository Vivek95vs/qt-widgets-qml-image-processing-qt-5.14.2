#ifndef NETWORKFOLDERCOPIER_H
#define NETWORKFOLDERCOPIER_H

#include <QObject>
#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QDebug>
#include <QTimer>
#include <QProcess>
#include <QThread>
#include <QMutex>
#include <QWaitCondition>
#include <QElapsedTimer>

class NetworkFolderCopier : public QObject
{
    Q_OBJECT

public:
    explicit NetworkFolderCopier(QObject *parent = nullptr);
    ~NetworkFolderCopier();

    void setVerbose(bool verbose) { m_verbose = verbose; }

signals:
    void finished();
    void error(const QString &errorMessage);
    void progress(const QString &message);
    void fileCopyStarted(const QString &fileName, qint64 size);
    void fileCopyProgress(qint64 bytesCopied, qint64 totalBytes);
    void fileCopyFinished(const QString &fileName, bool success);
    void directoryCreated(const QString &path);
    void authenticationRequired(const QString &networkPath);

public slots:
    void startCopy(const QString &sourcePath, const QString &destPath,
                   const QString &username = QString(),
                   const QString &password = QString(),
                   const QString &domain = QString());
    void cancel();

private slots:
    void handleAuthentication(const QString &networkPath);
    void retryWithAuthentication();

private:
    struct CopyJob {
        QString sourcePath;
        QString destPath;
        QString username;
        QString password;
        QString domain;
        bool useAuthentication;
    };

    CopyJob m_currentJob;
    bool m_verbose;
    bool m_cancelled;
    QMutex m_mutex;
    QWaitCondition m_waitCondition;
    QElapsedTimer m_totalTimer;
    QElapsedTimer m_fileTimer;

    // Statistics
    struct CopyStats {
        qint64 totalBytes = 0;
        qint64 copiedBytes = 0;
        int totalFiles = 0;
        int copiedFiles = 0;
        int totalDirs = 0;
        int createdDirs = 0;
        int failedItems = 0;
    } m_stats;

    bool copyDirectoryContents(const QString &sourcePath, const QString &destPath);
    bool copyFile(const QString &sourcePath, const QString &destPath);
    bool createDirectory(const QString &path);
    bool authenticateNetworkPath(const QString &path);
    QString formatSize(qint64 bytes);
    QString formatTime(qint64 ms);
    void log(const QString &message, bool always = false);
    void updateProgress();
    bool checkNetworkPath(const QString &path);
    QString normalizePath(const QString &path);
    bool isNetworkPath(const QString &path);
    QString extractServerFromPath(const QString &path);
};

#endif // NETWORKFOLDERCOPIER_H
