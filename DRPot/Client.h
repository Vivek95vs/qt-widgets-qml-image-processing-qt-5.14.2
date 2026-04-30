#ifndef CLIENT_H
#define CLIENT_H

#include <QObject>
#include <QTcpSocket>
#include <QThread>
#include <QString>
#include <QByteArray>
#include <QDir>
#include <QFile>
#include <QDateTime>
#include <QDebug>

class Client : public QObject
{
    Q_OBJECT

public:
    explicit Client(QObject *parent = nullptr);
    ~Client();

    // Static members converted to instance members
    int Communication_establish;
    int ReconnectFlag, ConnectDebounce;
    bool ConnectStatus;
    QByteArray tx_DR_data;
    QByteArray rx_DR_data;
    QByteArray CommandBuffer;
    QByteArray StatusBuffer;
    bool dummyTxStatus;
    bool eventTxRxTrigger;
    quint8 LogOtf;
    quint8 ProjectedSelected;

    int jog_acceleration_value;
    int jog_deceleration_value;
    int jog_velocity_value;
    QString MotionName;

    int PosControl_error;
    int SpeedControl_error;
    int error_encoder;
    quint8 Lateral_limit;
    quint8 Longitudinal_limit;
    quint8 Vertical_source_limit;
    quint8 Vertical_rotation_limit;
    quint8 Horizontal_rotation_limit;
    quint8 Vertical_detector_limit;
    quint8 Tilt_limit;
    int CurrentEncoderPosition_Counts;
    double CurrentEncoderPosition_Distance[8];
    quint8 DigitalInputFeedback[8];
    quint8 EnCoderReadFault[8];

    QString IP_Address;
    int PortNumber;
    int CollimatorCurrentposition;

    QString WorkdflowfilePath;
    QString WorkdflowfileName;
    QString CommunicationLogPath;

    enum motor_select
    {
        MotorNull = 0,
        Tube_lateral = 1,
        Tube_longitudinal,
        Tube_vertical,
        Tube_vertical_rotation,
        Tube_horizontal_rotation,
        Detector_vertical,
        Detector_tilt,
        Three_motors,
    };

    enum mode_select
    {
        ModeNull = 0,
        Auto_set_mode = 1,
        Manual_mode,
    };

    enum Collimator
    {
        X_jaw,
        Y_jaw,
    };

    void LPC_connect(const QString& _ipAddress, int _portNumber);
    void Dr_feedback_display_thread();
    void EventSendReceive();
    void _pauseEventTrigger();
    void CommunicationProcessData();
    void EncoderCurrentPosCountsToDistance();

    int AckRecieved;
    double DisplayCount1;
    QString Tube_Lateral_1, Tube_Lateral_2;
    QString Tube_Lateral_3, Tube_Lateral_4;
    double Tube_Lateral_int64_1, Tube_Lateral_int64_2;
    double Tube_Lateral_int64_3, Tube_Lateral_int64_4;
    int Tube_Lateral_int32_1, Tube_Lateral_int32_2;
    int Tube_Lateral_int32_3, Tube_Lateral_int32_4;
    int Z1;

    // Other motor display variables (similar pattern as above)
    // ...

    int Tube_Lateral_errorcode;
    int Tube_Longitudinal_errorcode;
    int Tube_Vertical_errorcode;
    int Tube_VerticalRotation_Errorcode;
    int Tube_HorizontalRotation_Errorcode;
    int Detector_Vertical_Errorcode;
    int Detector_Tilt_Errorcode;

    int ErrorCode[8];

    enum MotorNumber
    {
        Null = 0,
        One = 1,
        Two = 2,
        Three = 3,
        Four = 4,
        Five = 5,
        Six = 6,
        Seveen = 7,
    };

private:
    QTcpSocket *client_socket;
    QThread *feedbackThread;
};

#endif // CLIENT_H
