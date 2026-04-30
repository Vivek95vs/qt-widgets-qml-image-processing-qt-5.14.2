#ifndef PROTOCOLHANDLER_H
#define PROTOCOLHANDLER_H

#include <QObject>
#include <QByteArray>
#include <QVariantMap>

class ProtocolHandler : public QObject
{
    Q_OBJECT
public:
    explicit ProtocolHandler(QObject *parent = nullptr);

    QByteArray createMotorCommand(int motorType, int command, const QVariantMap &params = QVariantMap());
    QVariantMap parseFeedbackData(const QByteArray &data);

signals:
    void commandReady(const QByteArray &command);
    void feedbackParsed(const QVariantMap &data);

public slots:
    void processIncomingData(const QByteArray &data);

private:
    QByteArray calculateChecksum(const QByteArray &data);
    bool validateChecksum(const QByteArray &data);
    int distanceToPulseCalculation(int motorType, double distance);
};

#endif // PROTOCOLHANDLER_H
