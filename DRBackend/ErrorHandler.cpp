#include "ErrorHandler.h"

ErrorHandler::ErrorHandler(QObject *parent) : QObject(parent)
{
}

QVector<ErrorInfo> ErrorHandler::parseErrorCodes(const QByteArray &data)
{
    QVector<ErrorInfo> errors;

    if (data.size() < 4) return errors;

    for (int i = 0; i < 7; i++) {  // 7 motors
        int offset = i * 4;
        if (offset + 4 > data.size()) break;

        int errorCode = (static_cast<quint8>(data[offset]) << 24) |
                       (static_cast<quint8>(data[offset + 1]) << 16) |
                       (static_cast<quint8>(data[offset + 2]) << 8) |
                       static_cast<quint8>(data[offset + 3]);

        if (errorCode != 0) {
            ErrorInfo info;
            info.code = errorCode;
            info.description = errorCodeToString(errorCode);
            info.details = QString("Motor %1: %2").arg(i + 1).arg(info.description);
            info.isError = true;
            errors.append(info);
        }
    }

    m_currentErrors = errors;
    emit errorsUpdated(errors);
    return errors;
}

QString ErrorHandler::errorCodeToString(int code)
{
    switch(code) {
        case 0x00000001: return "Position Error Exceeds Limit";
        case 0x00008000: return "Motor Not Enabled";
        case 0x00000002: return "Reverse Prohibition Limit";
        case 0x00000004: return "Forward Prohibition Limit";
        case 0x00000008: return "Drive Over Temperature";
        case 0x00000010: return "Internal Voltage Error";
        case 0x00004000: return "Q Program Section Empty";
        case 0x00000020: return "Drive Over Voltage or Low Voltage";
        case 0x00100000: return "Drive Under Voltage";
        case 0x00020000: return "Safe Torque Prohibited";
        case 0x00000080: return "Drive Over Current";
        case 0x00002000: return "Motor Overload Status";
        case 0x00080000: return "Motor Speed Exceeds Limit";
        case 0x08000000: return "RS485 Communication Error";
        case 0x00000200: return "Motor Encoder Error";
        case 0x00200000: return "Emergency Stop";
        case 0x00000400: return "Serial Communication Abnormality";
        case 0x20000000: return "Motor Stall Protection";
        case 0x00000006: return "Stop Limit";
        case 0x04000000: return "Absolute Position Overflow";
        case 0x10000000: return "Absolute Encoder Multiturn Error";
        case 0x02000000: return "Absolute Position Lost";
        default: return QString("Unknown Error (0x%1)").arg(code, 0, 16);
    }
}

void ErrorHandler::processErrorData(const QByteArray &data)
{
    emit errorsUpdated(parseErrorCodes(data));
}
