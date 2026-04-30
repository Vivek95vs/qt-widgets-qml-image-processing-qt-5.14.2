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
#include "MotorParameters.h"

class Protocol : public QObject
{
    Q_OBJECT

public:
    explicit Protocol(QObject *parent = nullptr);

    struct DR_ProtocolIDs
    {
        static const quint8 Servomotor_start = 1;
        static const quint8 Servomotor_stop = 2;
        static const quint8 EncoderPositionReset = 3;
        static const quint8 collimator = 4;
        static const quint8 BootParameter = 5;
        static const quint8 MotorEnable = 6;
        static const quint8 MotorDisable = 7;
        static const quint8 AlaramReset = 8;
    };

    static QThread *Sequence_thread;
    static QThread *Dr_BoardFeedBack_thread;

    static QString WorkdflowfilePath;
    static QString WorkdflowfileName;
    static QString Log_Workflowpath;
    static QString ReadDataFromIA_Filepath;
    static QString ReadDataFromDatabase_Filepath;
    static quint8 MotorConnected;

    // Other static members...

    static QString CreateWorkflowLogfile(const QString& directory, const QString& name);
    static void WriteWorkflowToLogfile(const QString& path, const QString& data);
    static void ReadImageViewerData(const QString& path);
    static void ReadDatabaseValue(const QString& path);
    static void DRReq_ThreadStart();
    void BootParameter();
    // Other static methods...
    //New added
    // Motor parameters
    static QVector<double> TargetPosition;
    static QVector<double> StaticTolerance;
    static QVector<double> LinearDistancePerRotation;
    static QVector<int> PulsePerRotation;
    static QVector<quint8> MotorStartStopFlag;
    static QVector<int> TargetCounts;
    static QVector<int> velocity;
    static QVector<double> Reset_Position;

    quint8 Motor_Select;
    void motorEnableFunction(quint8 motor_num);
    void servoAutoStart(quint8 motor_num, const QVector<int>& vel, const QVector<double>& TargetPosition);
    void servomotorAutoStop(quint8 motor_num);
    int distanceToPulseCalculation(quint8 selectedMotor, double distance);

private:
    Client* m_client; // Add this member

};

#endif // PROTOCOL_H
