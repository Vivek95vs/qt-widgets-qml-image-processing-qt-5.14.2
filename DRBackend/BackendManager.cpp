#include "BackendManager.h"
#include <QDebug>
#include <QThread>

BackendManager::BackendManager(QObject *parent) : QObject(parent),
    m_boardConnection(new BoardConnection(this)),
    m_protocolHandler(new ProtocolHandler(this)),
    m_errorHandler(new ErrorHandler(this)),
    m_isConnected(false)
{
    connect(m_boardConnection, &BoardConnection::connected, this, &BackendManager::handleBoardConnected);
    connect(m_boardConnection, &BoardConnection::disconnected, this, &BackendManager::handleBoardDisconnected);
    connect(m_boardConnection, &BoardConnection::errorOccurred, this, &BackendManager::handleBoardError);
    connect(m_boardConnection, &BoardConnection::dataReceived, this, &BackendManager::handleFeedbackData);

    connect(m_protocolHandler, &ProtocolHandler::feedbackParsed, this, &BackendManager::feedbackReceived);
    connect(m_errorHandler, &ErrorHandler::errorsUpdated, this, [this](const QVector<ErrorInfo>& errors) {
        for (const auto& error : errors) {
            emit errorOccurred(error.details);
        }
    });

    m_heartbeatTimer = new QTimer(this);
    connect(m_heartbeatTimer, &QTimer::timeout, this, [this]() {
        if (m_isConnected) {
            QVariantMap params;
            QByteArray cmd = m_protocolHandler->createMotorCommand(0, 9, params); // Command 9 for heartbeat
            m_boardConnection->sendData(cmd);
        }
    });
    m_heartbeatTimer->start(10); // Every 5 seconds
}

BackendManager::~BackendManager()
{
    disconnectFromBoard();
    delete m_boardConnection;
    delete m_protocolHandler;
    delete m_errorHandler;
}

bool BackendManager::connectionStatus() const
{
    return m_isConnected;
}

void BackendManager::connectToBoard(const QString &ipAddress, int port)
{
    qDebug() << "Attempting to connect to" << ipAddress << ":" << port;
    if (m_isConnected) {
        qDebug() << "Already connected, disconnecting first";
        disconnectFromBoard();
    }
    m_boardConnection->connectToBoard(ipAddress, port);
}

void BackendManager::disconnectFromBoard()
{
    if (m_isConnected) {
        m_boardConnection->disconnectFromBoard();
    }
}

void BackendManager::resetMotor(int motorType)
{
    if (m_isConnected) {
        QVariantMap params;
        QByteArray cmd = m_protocolHandler->createMotorCommand(
            motorType, 2, params); // Command 2 for reset
        m_boardConnection->sendData(cmd);
    }
}

void BackendManager::resetAllMotors()
{
    if (m_isConnected) {
        for (int i = TubeLateral; i <= DetectorTilt; ++i) {
            resetMotor(i);
        }
    }
}

void BackendManager::startAutoMode()
{
    if (m_isConnected) {
        QVariantMap params;
        QByteArray cmd = m_protocolHandler->createMotorCommand(
            0, 3, params); // Command 3 for auto mode
        m_boardConnection->sendData(cmd);
    }
}

void BackendManager::emergencyStop()
{
    if (m_isConnected) {
        QVariantMap params;
        QByteArray cmd = m_protocolHandler->createMotorCommand(
            0, 4, params); // Command 4 for emergency stop
        m_boardConnection->sendData(cmd);
    }
}

void BackendManager::motorEnable(int motorType)
{
    if (m_isConnected) {
        QVariantMap params;
        params["motor"] = motorType;
        QByteArray cmd = m_protocolHandler->createMotorCommand(
            motorType, 6, params); // Command 6 for motor enable
        m_boardConnection->sendData(cmd);
    }
}

void BackendManager::motorDisable(int motorType)
{
    if (m_isConnected) {
        QVariantMap params;
        params["motor"] = motorType;
        QByteArray cmd = m_protocolHandler->createMotorCommand(
            motorType, 7, params); // Command 7 for motor disable
        m_boardConnection->sendData(cmd);
    }
}

void BackendManager::servoAutoStart(int motorType, const QVariantList &velocities, const QVariantList &targetPositions)
{
    if (m_isConnected && velocities.size() >= 7 && targetPositions.size() >= 7) {
        QVariantMap params;
        params["mode"] = AutoSetMode;
        params["motor"] = motorType;

        for (int i = 0; i < 7; i++) {
            params[QString("velocity%1").arg(i)] = velocities[i].toInt() * 240;
            params[QString("target%1").arg(i)] = targetPositions[i].toInt();
        }

        QByteArray cmd = m_protocolHandler->createMotorCommand(
            motorType, 1, params); // Command 1 for servo auto start
        m_boardConnection->sendData(cmd);
    }
}

void BackendManager::servoAutoStop(int motorType)
{
    if (m_isConnected) {
        QVariantMap params;
        params["motor"] = motorType;
        QByteArray cmd = m_protocolHandler->createMotorCommand(
            motorType, 2, params); // Command 2 for servo auto stop
        m_boardConnection->sendData(cmd);
    }
}

void BackendManager::encoderPositionReset(int motorType, const QVariantList &resetPositions)
{
    if (m_isConnected && resetPositions.size() >= 7) {
        QVariantMap params;
        params["motor"] = motorType;

        for (int i = 0; i < 7; i++) {
            params[QString("reset%1").arg(i)] = resetPositions[i].toInt();
        }

        QByteArray cmd = m_protocolHandler->createMotorCommand(
            motorType, 3, params); // Command 3 for encoder position reset
        m_boardConnection->sendData(cmd);
    }
}

void BackendManager::alarmReset(int motorType)
{
    if (m_isConnected) {
        QVariantMap params;
        params["motor"] = motorType;
        QByteArray cmd = m_protocolHandler->createMotorCommand(
            motorType, 8, params); // Command 8 for alarm reset
        m_boardConnection->sendData(cmd);
    }
}

void BackendManager::bootParameters()
{
    if (m_isConnected) {
        QVariantMap params;
        // Add all boot parameters here
        QByteArray cmd = m_protocolHandler->createMotorCommand(
            0, 5, params); // Command 5 for boot parameters
        m_boardConnection->sendData(cmd);
    }
}

void BackendManager::handleBoardConnected()
{
    m_isConnected = true;
    emit connectionStatusChanged(true);
}

void BackendManager::handleBoardDisconnected()
{
    m_isConnected = false;
    emit connectionStatusChanged(false);
}

void BackendManager::handleBoardError(const QString &error)
{
    m_isConnected = false;
    emit connectionStatusChanged(false);
    emit errorOccurred(error);
}

void BackendManager::handleFeedbackData(const QByteArray &data)
{
    if (data.isEmpty()) {
        qWarning() << "Received empty feedback data";
        return;
    }

    qDebug()<<"recive data"<<data.size()<<data;
    // First process errors
    m_errorHandler->processErrorData(data);

    // Then process normal feedback
    try {
        m_protocolHandler->processIncomingData(data);
    } catch (const std::exception& e) {
        qWarning() << "Error processing feedback data:" << e.what();
        emit errorOccurred(tr("Invalid data received from device"));
    }
}
