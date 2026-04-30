#include "ProtocolHandler.h"
#include <QDebug>

// Initialize constants
const int ProtocolHandler::PULSE_PER_ROTATION[7] = { /* your values */ };
const double ProtocolHandler::LINEAR_DISTANCE_PER_ROTATION[7] = { /* your values */ };

ProtocolHandler::ProtocolHandler(QObject *parent) : QObject(parent),
    m_client(nullptr),
    m_running(false),
    m_communicationEstablished(false),
    m_eventTxRxTrigger(false),
    m_dummyTxStatus(true),
    m_reconnectFlag(0),
    m_connectDebounce(0),
    m_logOtf(0)
{
    // Initialize arrays
    for (int i = 0; i < 7; i++) {
        m_displayCounts[i] = 0;
        m_errorCodes[i] = 0;
        m_digitalInputFeedback[i] = 0;
        m_currentEncoderDistance[i] = 0.0;
    }

    // Set log path
    m_communicationLogPath = QDir::currentPath() + "/communication_log.txt";

    // Move timer to thread
    m_feedbackTimer.moveToThread(&m_feedbackThread);
    m_reconnectTimer.moveToThread(&m_feedbackThread);

    // Connect signals
    connect(&m_feedbackTimer, &QTimer::timeout, this, &ProtocolHandler::feedbackDisplay);
    connect(&m_reconnectTimer, &QTimer::timeout, this, &ProtocolHandler::attemptReconnect);
    connect(m_client, &SocketClient::dataReceived, this, &ProtocolHandler::processReceivedData);

    // Start thread
    m_feedbackThread.start();
}

ProtocolHandler::~ProtocolHandler()
{
    stopThreads();
    m_feedbackThread.quit();
    m_feedbackThread.wait();
}

void ProtocolHandler::bootParameters()
{
    if (!m_client || !m_client->connected()) {
        return;
    }

    QByteArray commandBuffer;
    commandBuffer.append(static_cast<char>(0x0B)); // ByteNumber = 11
    commandBuffer.append(static_cast<char>(0x01)); // DR_ProtocolIDs.BootParameter

    // Add slave addresses and parameters (simplified)
    for (int i = 1; i <= 7; i++) {
        commandBuffer.append(static_cast<char>(i));
    }

    // Add motor parameters
    // ... (add your specific parameters here)

    m_client->sendData(commandBuffer);
}

void ProtocolHandler::startThreads()
{
    if (m_running) return;
    m_running = true;

    QMetaObject::invokeMethod(&m_feedbackTimer, [this]() {
        m_feedbackTimer.start(10); // 10ms interval like original
    }, Qt::QueuedConnection);
}

void ProtocolHandler::stopThreads()
{
    m_running = false;
    m_feedbackTimer.stop();
    m_reconnectTimer.stop();
}

void ProtocolHandler::feedbackDisplay()
{
    if (!m_running) return;

    if (m_communicationEstablished) {
        try {
            if (!m_eventTxRxTrigger) {
                m_dummyTxStatus = false;

                // Send command and receive data
                QByteArray commandBuffer; // Should be populated with your command
                m_client->sendData(commandBuffer);
                // Data will be processed in processReceivedData slot

                m_communicationEstablished = true;
                m_dummyTxStatus = true;

                if (m_logOtf == 1) {
                    logConnectionEvent("reconnected at...");
                    m_logOtf = 0;
                }
            }
        } catch (...) {
            m_communicationEstablished = false;
            m_eventTxRxTrigger = false;
            emit connectionStatusChanged(false);
        }
    } else {
        if (m_logOtf == 0) {
            logConnectionEvent("connection lost at...");
            m_logOtf = 1;
        }
    }

    if (m_reconnectFlag == 1) {
        m_connectDebounce++;
        if (m_connectDebounce > 100) {
            m_connectDebounce = 0;
            m_reconnectFlag = 2;
            m_client->connectToServer();
        }
    }
}

void ProtocolHandler::processReceivedData(const QByteArray &data)
{
    communicationProcessData(data);
    emit feedbackUpdated("Data processed successfully");
}

void ProtocolHandler::communicationProcessData(const QByteArray &statusBuffer)
{
    if (statusBuffer.size() < 50) return; // Adjust based on your expected size

    int index = 1; // Starting index

    // Process digital input feedback
    for (int i = 0; i < 7; i++) {
        m_digitalInputFeedback[i] = static_cast<unsigned char>(statusBuffer[index++]);
    }

    // Process motor positions (simplified example for motor 1)
    m_displayCounts[0] = (static_cast<unsigned char>(statusBuffer[index+3]) << 24) |
                         (static_cast<unsigned char>(statusBuffer[index+2]) << 16) |
                         (static_cast<unsigned char>(statusBuffer[index+1]) << 8) |
                         (static_cast<unsigned char>(statusBuffer[index]));
    index += 4;

    // Repeat for motors 2-7...
    // ...

    // Process error codes
    for (int i = 0; i < 7; i++) {
        m_errorCodes[i] = (static_cast<unsigned char>(statusBuffer[index]) << 24) |
                          (static_cast<unsigned char>(statusBuffer[index+1]) << 16) |
                          (static_cast<unsigned char>(statusBuffer[index+2]) << 8) |
                          static_cast<unsigned char>(statusBuffer[index+3]);
        index += 4;
    }

    // Process remaining data...
    // ...

    encoderCountsToDistance();
    emit displayCountsChanged();
    emit errorCodesChanged();
}

void ProtocolHandler::encoderCountsToDistance()
{
    for (int i = 0; i < 7; i++) {
        m_currentEncoderDistance[i] = (m_displayCounts[i] * LINEAR_DISTANCE_PER_ROTATION[i]) / PULSE_PER_ROTATION[i];
    }
}

void ProtocolHandler::attemptReconnect()
{
    if (m_reconnectFlag == 1) {
        m_client->connectToServer();
    }
}

void ProtocolHandler::logConnectionEvent(const QString &message)
{
    QDir().mkpath(QFileInfo(m_communicationLogPath).absolutePath());

    QFile file(m_communicationLogPath);
    if (file.open(QIODevice::Append | QIODevice::Text)) {
        QTextStream stream(&file);
        stream << message << " " << QDateTime::currentDateTime().toString() << "\n";
        file.close();
    }
}

SocketClient* ProtocolHandler::client() const { return m_client; }
void ProtocolHandler::setClient(SocketClient *client)
{
    if (m_client != client) {
        if (m_client) {
            disconnect(m_client, &SocketClient::dataReceived, this, &ProtocolHandler::processReceivedData);
        }

        m_client = client;

        if (m_client) {
            connect(m_client, &SocketClient::dataReceived, this, &ProtocolHandler::processReceivedData);
            connect(m_client, &SocketClient::connectionChanged, this, [this](bool connected) {
                m_communicationEstablished = connected;
                emit connectionStatusChanged(connected);
            });
        }

        emit clientChanged();
    }
}

QString ProtocolHandler::errorCodes() const
{
    QStringList codes;
    for (int i = 0; i < 7; i++) {
        codes.append(QString::number(m_errorCodes[i], 16));
    }
    return codes.join(", ");
}
