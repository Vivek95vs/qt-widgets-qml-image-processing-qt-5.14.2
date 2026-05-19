#ifndef DICOMMANAGER_H
#define DICOMMANAGER_H

#include <QObject>
#include <QThread>
#include <QSettings>
#include <QPointer>
#include <QVariantList>
#include <QVariantMap>
#include <QHash>

// Forward declarations for DCMTK
#include "dcmtk/config/osconfig.h"
#include "dcmtk/dcmnet/scu.h"
#include "dcmtk/dcmdata/dcfilefo.h"
#include "dcmtk/dcmnet/dimse.h"

struct ServerConfig {
    QString aeTitle;
    QString hostname;
    int port;
    QString name;
};

class DicomWorker : public QObject {
    Q_OBJECT
public:
    explicit DicomWorker(QObject *parent = nullptr);

public slots:
    void initialize(const QString& configPath);
    void queryPACS(const QString& patientId = QString());
    void retrieveStudy(const QString& studyUID, const QString& savePath);
    void sendDICOMFile(const QString& filePath, const QString& serverName);
    void testConnection(const QString& serverName);

    void stop();

signals:
    void progressUpdated(int percent, const QString& message);
    void studyListReady(const QVariantList& studies);
    void transferCompleted(bool success, const QString& message);
    void logMessage(const QString& level, const QString& message);

private:
    bool createAssociation(const ServerConfig& server);
    void closeAssociation();

    DcmSCU* m_scu;
    bool m_isRunning;
    QString m_localAETitle;
    int m_localPort;
    QSettings* m_settings;
    QHash<QString, ServerConfig> m_servers;
};

class DicomManager : public QObject {
    Q_OBJECT
    Q_PROPERTY(bool isTransferring READ isTransferring NOTIFY transferringChanged)
    Q_PROPERTY(QString currentStatus READ currentStatus NOTIFY statusChanged)
    Q_PROPERTY(QVariantList studies READ studies NOTIFY studiesChanged)
    Q_PROPERTY(QVariantList servers READ servers NOTIFY serversChanged)

public:
    explicit DicomManager(QObject *parent = nullptr);
    ~DicomManager();

    bool isTransferring() const { return m_isTransferring; }
    QString currentStatus() const { return m_currentStatus; }
    QVariantList studies() const { return m_studies; }
    QVariantList servers() const { return m_serversList; }

    Q_INVOKABLE void loadConfiguration(const QString& configPath);
    Q_INVOKABLE void queryStudies(const QString& patientId = QString());
    Q_INVOKABLE void downloadStudy(const QString& studyUID, const QString& savePath);
    Q_INVOKABLE void sendToPACS(const QString& filePath, const QString& serverName);
    Q_INVOKABLE void sendDirectoryToPACS(const QString& directoryPath, const QString& serverName);
    Q_INVOKABLE void verifyConnection(const QString& serverName);
    Q_INVOKABLE void testConnection(const QString& serverName);

signals:
    void transferringChanged();
    void statusChanged();
    void studiesChanged();
    void serversChanged();
    void studyQueryCompleted(const QVariantList& studies);
    void operationCompleted(bool success, const QString& message);
    void logMessage(const QString& level, const QString& message);
    void progressChanged(int percent, const QString& message);

private slots:
    void onStudyListReady(const QVariantList& studies);
    void onTransferCompleted(bool success, const QString& message);
    void onLogMessage(const QString& level, const QString& message);
    void onProgressUpdated(int percent, const QString& message);
    void onWorkerFinished();

private:
    void setTransferring(bool transferring);
    void setStatus(const QString& status);
    void updateStudies(const QVariantList& studies);

    QThread* m_workerThread;
    DicomWorker* m_worker;
    bool m_isTransferring;
    QString m_currentStatus;
    QVariantList m_studies;
    QVariantList m_serversList;
    QHash<QString, ServerConfig> m_serverConfigs;
    ServerConfig m_localConfig;
};

#endif // DICOMMANAGER_H
