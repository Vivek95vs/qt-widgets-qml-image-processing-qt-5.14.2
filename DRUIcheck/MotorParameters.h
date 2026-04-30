#ifndef MOTORPARAMETERS_H
#define MOTORPARAMETERS_H

#include <QByteArray>

class MotorParameters
{
public:
    struct MotorAddressParameter {
        static QByteArray PositionmodeParameters;
        static QByteArray MotorTargetPosition;
        static QByteArray MotorEnable;
        static QByteArray MotorDisable;
        static QByteArray MotorStart;
        static QByteArray MotorStop;
        static QByteArray JogSlaveData;
        static QByteArray Alarmreset;
        static QByteArray AlarmcodeRead;
        static QByteArray EncoderPositionReset;
    };

    enum Slave_Address {
        Tube_Lateral = 0x01,
        Tube_Longitudinal = 0x02,
        Tube_Vertical = 0x03,
        Tube_Vertical_rotation = 0x04,
        Tube_Horizontal_rotation = 0x05,
        Detector_Vertical = 0x06,
        Detector_tilt = 0x07
    };

    enum DR_ProtocolIDs {
        Servomotor_start = 1,
        Servomotor_stop = 2,
        EncoderPositionReset = 3,
        collimator = 4,
        BootParameter = 5,
        MotorEnable = 6,
        MotorDisable = 7,
        AlaramReset = 8
    };
};

#endif // MOTORPARAMETERS_H
