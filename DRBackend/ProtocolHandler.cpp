#include "ProtocolHandler.h"
#include <QDebug>
#include <QtEndian>

ProtocolHandler::ProtocolHandler(QObject *parent) : QObject(parent)
{
}

QByteArray ProtocolHandler::createMotorCommand(int motorType, int command, const QVariantMap &params)
{
    QByteArray cmd;
    cmd.append(static_cast<char>(0xAA));  // Start byte
    cmd.append(static_cast<char>(motorType));
    cmd.append(static_cast<char>(command));

    // Add parameters based on command type
    switch (command) {
    case 1:  // Move command
        for (int i = 0; i < 7; i++) {
            int velocity = params[QString("velocity%1").arg(i)].toInt();
            int position = params[QString("target%1").arg(i)].toInt();

            cmd.append(static_cast<char>((velocity >> 24) & 0xFF));
            cmd.append(static_cast<char>((velocity >> 16) & 0xFF));
            cmd.append(static_cast<char>((velocity >> 8) & 0xFF));
            cmd.append(static_cast<char>(velocity & 0xFF));

            cmd.append(static_cast<char>((position >> 24) & 0xFF));
            cmd.append(static_cast<char>((position >> 16) & 0xFF));
            cmd.append(static_cast<char>((position >> 8) & 0xFF));
            cmd.append(static_cast<char>(position & 0xFF));
        }
        break;

    case 3:  // Encoder position reset
        for (int i = 0; i < 7; i++) {
            int position = params[QString("reset%1").arg(i)].toInt();

            cmd.append(static_cast<char>((position >> 24) & 0xFF));
            cmd.append(static_cast<char>((position >> 16) & 0xFF));
            cmd.append(static_cast<char>((position >> 8) & 0xFF));
            cmd.append(static_cast<char>(position & 0xFF));
        }
        break;

    case 6:  // Motor enable
    case 7:  // Motor disable
    case 8:  // Alarm reset
        cmd.append(static_cast<char>(params["motor"].toInt()));
        break;
    }

    // Calculate and append checksum
    QByteArray checksum = calculateChecksum(cmd);
    cmd.append(checksum);

    emit commandReady(cmd);
    return cmd;
}

QVariantMap ProtocolHandler::parseFeedbackData(const QByteArray &data)
{
    QVariantMap feedback;

    qDebug() << "Raw feedback data (" << data.size() << "bytes):" << data.toHex();

    if (data.size() < 8 || !validateChecksum(data)) {
        qWarning() << "Invalid feedback data or checksum";
        return feedback;
    }

    // Parse motor data (7 motors)
    for (int i = 0; i < 7; i++) {
        int offset = i * 12;
        if (offset + 12 > data.size()) break;

        QVariantMap motorData;

        // Digital input feedback
        motorData["digitalInput"] = static_cast<quint8>(data[offset]);

        // Position (4 bytes)
        int position = (static_cast<quint8>(data[offset + 1]) << 24) |
                      (static_cast<quint8>(data[offset + 2]) << 16) |
                      (static_cast<quint8>(data[offset + 3]) << 8) |
                      static_cast<quint8>(data[offset + 4]);
        motorData["position"] = position;

        // Status
        motorData["status"] = QString::number(static_cast<quint8>(data[offset + 5]));

        // Error code (4 bytes)
        int errorCode = (static_cast<quint8>(data[offset + 6]) << 24) |
                       (static_cast<quint8>(data[offset + 7]) << 16) |
                       (static_cast<quint8>(data[offset + 8]) << 8) |
                       static_cast<quint8>(data[offset + 9]);
        motorData["errorCode"] = errorCode;

        feedback[QString::number(i + 1)] = motorData;
    }

    // Parse collimator position (if available)
    if (data.size() >= 7 * 12 + 4) {
        int collimatorPos = (static_cast<quint8>(data[7 * 12]) << 24) |
                           (static_cast<quint8>(data[7 * 12 + 1]) << 16) |
                           (static_cast<quint8>(data[7 * 12 + 2]) << 8) |
                           static_cast<quint8>(data[7 * 12 + 3]);
        feedback["collimatorPosition"] = collimatorPos;
    }

    // Parse ACK (if available)
    if (data.size() >= 7 * 12 + 5) {
        feedback["ackReceived"] = static_cast<quint8>(data[7 * 12 + 4]);
    }

    emit feedbackParsed(feedback);
    return feedback;
}

void ProtocolHandler::processIncomingData(const QByteArray &data)
{
    emit feedbackParsed(parseFeedbackData(data));
}

QByteArray ProtocolHandler::calculateChecksum(const QByteArray &data)
{
    quint8 checksum = 0;
    for (char byte : data) {
        checksum ^= static_cast<quint8>(byte);
    }
    return QByteArray(1, static_cast<char>(checksum));
}

bool ProtocolHandler::validateChecksum(const QByteArray &data)
{
    if (data.isEmpty()) return false;

    quint8 checksum = 0;
    const int dataSize = data.size();

    // Calculate checksum for all bytes except the last one
    for (int i = 0; i < dataSize - 1; i++) {
        checksum ^= static_cast<quint8>(data.at(i));
    }

    // Compare with the last byte (checksum)
    return checksum == static_cast<quint8>(data.at(dataSize - 1));
}

int ProtocolHandler::distanceToPulseCalculation(int motorType, double distance)
{
    // These values should match your C# implementation
    static const int pulsePerRotation[] = {10000, 10000, 10000, 10000, 10000, 10000, 10000};
    static const double linearDistancePerRotation[] = {10.0, 10.0, 10.0, 10.0, 10.0, 10.0, 10.0};

    if (motorType < 1 || motorType > 7) return 0;

    double pulses = (distance * pulsePerRotation[motorType - 1]) / linearDistancePerRotation[motorType - 1];
    return static_cast<int>(qRound(pulses));
}
