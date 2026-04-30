#include <QCoreApplication>
#include <QDebug>
#include "NetworkFolderCopier.h"

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);

    // HARDCODED PATHS - MODIFY THESE VALUES
    // ======================================
    // CORRECT PATH based on diagnostics:
    // The entire D: drive is shared as "D"
    QString sourcePath = "\\\\192.168.10.156\\D\\DICOM";  // ✅ CORRECT

    QString destPath = "D:\\copypath";
    QString username = "";        // Leave empty if no authentication needed
    QString password = "";         // Leave empty if no authentication needed
    QString domain = "";           // Leave empty if no domain
    bool verbose = true;           // Set to false for less output
    // ======================================

    qInfo() << "Starting folder copy with hardcoded paths...";
    qInfo() << "Source (remote D: drive shared as 'D'):" << sourcePath;
    qInfo() << "Destination:" << destPath;

    // Verify the path exists before starting
    QDir testDir(sourcePath);
    if (testDir.exists()) {
        qInfo() << "✓ Source path is accessible!";
        qInfo() << "Found" << testDir.entryList(QDir::Files).size() << "files";
    } else {
        qWarning() << "⚠ Warning: Source path not accessible. Check if:";
        qWarning() << "  - Remote PC is online";
        qWarning() << "  - Share name is 'D' (not 'dummymotion')";
        qWarning() << "  - You have permission to access";
    }

    NetworkFolderCopier copier;
    copier.setVerbose(verbose);

    // Connect signals
    QObject::connect(&copier, &NetworkFolderCopier::finished, &app, &QCoreApplication::quit);
    QObject::connect(&copier, &NetworkFolderCopier::error, [](const QString &error) {
        qCritical() << "Error:" << error;
        QCoreApplication::exit(1);
    });

    // Start the copy operation
    QMetaObject::invokeMethod(&copier, "startCopy", Qt::QueuedConnection,
                              Q_ARG(QString, sourcePath),
                              Q_ARG(QString, destPath),
                              Q_ARG(QString, username),
                              Q_ARG(QString, password),
                              Q_ARG(QString, domain));

    return app.exec();
}
