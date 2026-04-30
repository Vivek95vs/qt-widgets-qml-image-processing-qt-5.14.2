#ifndef BACKENDMANAGER_H
#define BACKENDMANAGER_H

#include <QObject>
#include <QAbstractListModel>
#include "BoardConnection.h"
#include "ProtocolHandler.h"
#include "ErrorHandler.h"

class BackendManager : public QObject
{
    Q_OBJECT
    Q_PROPERTY(bool connectionStatus READ connectionStatus NOTIFY connectionStatusChanged)

public:
    explicit BackendManager(QObject *parent = nullptr);
    ~BackendManager();

    enum MotorType {
        TubeLateral = 1,
        TubeLongitudinal,
        TubeVertical,
        TubeVerticalRotation,
        TubeHorizontalRotation,
        DetectorVertical,
        DetectorTilt
    };
    Q_ENUM(MotorType)

    enum ModeSelect {
        NullMode = 0,
        AutoSetMode = 1,
        ManualMode = 2
    };
    Q_ENUM(ModeSelect)

    enum MotorDirection {
        ClockWise = 0,
        CounterClockwise = 1
    };
    Q_ENUM(MotorDirection)

    bool connectionStatus() const;

public slots:
    void connectToBoard(const QString &ipAddress, int port);
    void disconnectFromBoard();
    void resetMotor(int motorType);
    void resetAllMotors();
    void startAutoMode();
    void emergencyStop();
    void motorEnable(int motorType);
    void motorDisable(int motorType);
    void servoAutoStart(int motorType, const QVariantList &velocities, const QVariantList &targetPositions);
    void servoAutoStop(int motorType);
    void encoderPositionReset(int motorType, const QVariantList &resetPositions);
    void alarmReset(int motorType);
    void bootParameters();

signals:
    void connectionStatusChanged(bool status);
    void errorOccurred(const QString &message);
    void feedbackReceived(const QVariantMap &data);
    void motorStatusChanged(int motorType, int status);
    void positionUpdated(int motorType, double position);

private slots:
    void handleBoardConnected();
    void handleBoardDisconnected();
    void handleBoardError(const QString &error);
    void handleFeedbackData(const QByteArray &data);

private:
    BoardConnection *m_boardConnection;
    ProtocolHandler *m_protocolHandler;
    QTimer *m_heartbeatTimer;
    ErrorHandler *m_errorHandler;
    bool m_isConnected;
};

#endif // BACKENDMANAGER_H
