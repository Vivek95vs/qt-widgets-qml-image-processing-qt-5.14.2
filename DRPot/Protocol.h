#ifndef PROTOCOL_H
#define PROTOCOL_H

#include <QObject>
#include <QThread>
#include <QString>
#include <QByteArray>
#include <QFile>
#include <QDir>
#include <QDateTime>
#include "Client.h"

class Protocol : public QObject
{
    Q_OBJECT

public:
    explicit Protocol(QObject *parent = nullptr);

    struct DR_ProtocolIDs
    {
        static const quint8 Servomotor_start = 1;
        static const quint8 Servomotor_stop = 2;
        // Other protocol IDs...
    };

    static QThread *Sequence_thread;
    static QThread *Dr_BoardFeedBack_thread;

    static QString WorkdflowfilePath;
    static QString WorkdflowfileName;
    static QString Log_Workflowpath;
    static QString ReadDataFromIA_Filepath;
    static QString ReadDataFromDatabase_Filepath;

    // Other static members...

    static QString CreateWorkflowLogfile(const QString& directory, const QString& name);
    static void WriteWorkflowToLogfile(const QString& path, const QString& data);
    static void ReadImageViewerData(const QString& path);
    static void ReadDatabaseValue(const QString& path);
    static void DRReq_ThreadStart();
    static void BootParameter();
    // Other static methods...
};

#endif // PROTOCOL_H
