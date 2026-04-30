#ifndef ERRORCODE_H
#define ERRORCODE_H

#include <QString>

class ErrorCode
{
public:
    struct AlarmCode
    {
        static const int PositionErrorExeedsLimit = 1;
        static const int MotorNotEnable = 32768;
        // Other alarm codes...
    };

    struct AlarmCode_String
    {
        static const QString PositionErrorExeedsLimit;
        static const QString MotorNotEnable;
        // Other alarm code strings...
    };
};

#endif // ERRORCODE_H
