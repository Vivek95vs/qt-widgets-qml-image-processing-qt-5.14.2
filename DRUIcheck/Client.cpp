#include "Client.h"
#include "Protocol.h"
#include <QCoreApplication>
#include <QThread>
#include <QTimer>

Client::Client(QObject *parent) : QObject(parent),
    Communication_establish(0),
    ReconnectFlag(0),
    ConnectDebounce(0),
    ConnectStatus(false),
    dummyTxStatus(true),
    eventTxRxTrigger(false),
    LogOtf(0),
    ProjectedSelected(0),
    client_socket(new QTcpSocket(this))
{
    tx_DR_data.fill(0, 500);
    rx_DR_data.fill(0, 500);
    CommandBuffer.fill(0, 500);
    StatusBuffer.fill(0, 500);

    // Initialize other members...
    IP_Address = "192.168.3.20";
    PortNumber = 4444;
    CollimatorCurrentposition = 0;

    WorkdflowfilePath = "D:/Digital_Radiotheraphy";
    WorkdflowfileName = "Workflow.txt";
    CommunicationLogPath = "D:/Digital_Radiotheraphy/Communication.txt";

    // Connect socket signals
    connect(client_socket, &QTcpSocket::connected, this, [this]() {
        Communication_establish = 1;
        ReconnectFlag = 0;
    });

    connect(client_socket, &QTcpSocket::disconnected, this, [this]() {
        Communication_establish = 0;
    });

    connect(client_socket, QOverload<QAbstractSocket::SocketError>::of(&QTcpSocket::error), this, [this]() {
        Communication_establish = 0;
        eventTxRxTrigger = false;
        qDebug() << "Socket error:" << client_socket->errorString();
    });

    // Start feedback thread
    feedbackThread = QThread::create([this]() { Dr_feedback_display_thread(); });
    feedbackThread->start();
}

Client::~Client()
{
    feedbackThread->quit();
    feedbackThread->wait();
    delete feedbackThread;
    delete client_socket;
}

void Client::LPC_connect(const QString& _ipAddress, int _portNumber)
{
    IP_Address = _ipAddress;
    PortNumber = _portNumber;

    if (ConnectStatus)
    {
        client_socket->connectToHost(IP_Address, PortNumber);
        if (!client_socket->waitForConnected(500))
        {
            Communication_establish = 0;
            eventTxRxTrigger = false;
            qDebug() << "Connection failed";
        }
    }
    else
    {
        if (client_socket->state() != QAbstractSocket::UnconnectedState)
        {
            Communication_establish = 0;
            client_socket->disconnectFromHost();
            if (client_socket->state() != QAbstractSocket::UnconnectedState)
            {
                client_socket->waitForDisconnected();
            }
        }
    }
}

void Client::Dr_feedback_display_thread()
{
    while (true)
    {
        QThread::msleep(10);
        if (Communication_establish == 1)
        {
            try
            {
                if (!eventTxRxTrigger)
                {
                    dummyTxStatus = false;

                    client_socket->write(CommandBuffer);
                    if (client_socket->waitForBytesWritten(500))
                    {
                        if (client_socket->waitForReadyRead(800))
                        {
                            rx_DR_data = client_socket->readAll();
                            CommunicationProcessData();
                            Communication_establish = 1;
                            dummyTxStatus = true;

                            if (LogOtf == 1)
                            {
                                QDir dir(WorkdflowfilePath);
                                if (!dir.exists())
                                {
                                    dir.mkpath(".");
                                }

                                QFile file(CommunicationLogPath);
                                if (file.open(QIODevice::Append))
                                {
                                    QTextStream stream(&file);
                                    stream << "reconnected at... " << QDateTime::currentDateTime().toString() << Qt::endl;
                                    file.close();
                                }
                                LogOtf = 0;
                            }
                        }
                    }
                }
            }
            catch (...)
            {
                Communication_establish = 0;
                eventTxRxTrigger = false;
            }
        }
        else
        {
            if (LogOtf == 0)
            {
                QDir dir(WorkdflowfilePath);
                if (!dir.exists())
                {
                    dir.mkpath(".");
                }

                QFile file(CommunicationLogPath);
                if (file.open(QIODevice::Append))
                {
                    QTextStream stream(&file);
                    stream << "connection lost at... " << QDateTime::currentDateTime().toString() << Qt::endl;
                    file.close();
                }
                LogOtf = 1;
            }
        }

        if (ReconnectFlag == 1)
        {
            ConnectDebounce++;
            if (ConnectDebounce > 100)
            {
                ConnectDebounce = 0;
                ReconnectFlag = 2;
                LPC_connect(IP_Address, PortNumber);
            }
        }
    }
}

void Client::EventSendReceive()
{
    if (Communication_establish == 1)
    {
        try
        {
            while (!dummyTxStatus);
            eventTxRxTrigger = true;
            QThread::msleep(50);

            client_socket->write(CommandBuffer);
            if (client_socket->waitForBytesWritten(500))
            {
                if (client_socket->waitForReadyRead(800))
                {
                    rx_DR_data = client_socket->readAll();
                    CommunicationProcessData();
                    Communication_establish = 1;
                    eventTxRxTrigger = false;
                }
            }
        }
        catch (...)
        {
            eventTxRxTrigger = false;
            Communication_establish = 0;
            qDebug() << "Socket Exception EventSendReceive";
        }
    }
}

void Client::_pauseEventTrigger()
{
    if (Communication_establish == 1)
    {
        while (!dummyTxStatus);
        eventTxRxTrigger = true;
    }
}
void Client::CommunicationProcessData()
{
    int Index = 1;

    qDebug() << "Raw rx_DR_data size:" << rx_DR_data.size();
    qDebug() << "Hex dump of first 32 bytes:" << rx_DR_data.left(32).toHex(' ');

    if (rx_DR_data.isEmpty()) {
        qWarning() << "Received empty data!";
        return;
    }

    // Make sure StatusBuffer is being set properly
    StatusBuffer = rx_DR_data;  // Or whatever your protocol requires
    qDebug() << "StatusBuffer hex:" << StatusBuffer.toHex(' ');

    qDebug() << "StatusBuffer Contents (Hex):" << StatusBuffer.toHex();

    DigitalInputFeedback[(quint8)motor_select::Tube_lateral] = StatusBuffer[Index++];
    DigitalInputFeedback[(quint8)motor_select::Tube_longitudinal] = StatusBuffer[Index++];
    DigitalInputFeedback[(quint8)motor_select::Tube_vertical] = StatusBuffer[Index++];
    DigitalInputFeedback[(quint8)motor_select::Tube_vertical_rotation] = StatusBuffer[Index++];
    DigitalInputFeedback[(quint8)motor_select::Tube_horizontal_rotation] = StatusBuffer[Index++];
    DigitalInputFeedback[(quint8)motor_select::Detector_vertical] = StatusBuffer[Index++];
    DigitalInputFeedback[(quint8)motor_select::Detector_tilt] = StatusBuffer[Index++];

    Tube_Lateral_1 = QString::number(StatusBuffer[Index++], 16);
    Tube_Lateral_2 = QString::number(StatusBuffer[Index++], 16);
    Tube_Lateral_3 = QString::number(StatusBuffer[Index++], 16);
    Tube_Lateral_4 = QString::number(StatusBuffer[Index++], 16);

    Tube_Lateral_int64_1 = Tube_Lateral_1.toLongLong(nullptr, 16);
    Tube_Lateral_int64_2 = Tube_Lateral_2.toLongLong(nullptr, 16);
    Tube_Lateral_int64_3 = Tube_Lateral_3.toLongLong(nullptr, 16);
    Tube_Lateral_int64_4 = Tube_Lateral_4.toLongLong(nullptr, 16);

    Tube_Lateral_int32_1 = (int)Tube_Lateral_int64_1;
    Tube_Lateral_int32_2 = (int)Tube_Lateral_int64_2;
    Tube_Lateral_int32_3 = (int)Tube_Lateral_int64_3;
    Tube_Lateral_int32_4 = (int)Tube_Lateral_int64_4;

    Z1 = (int)(Tube_Lateral_int32_4 | (Tube_Lateral_int32_3 << 8) | (Tube_Lateral_int32_2 << 16) | (Tube_Lateral_int32_1 << 24));
    DisplayCount1 = (double)Z1;

    // Similar processing for other motors...

    // Process error codes
    for (int i = 0; i < 7; i++)
    {
        ErrorCode[i] = ((StatusBuffer[Index++] << 24) | (StatusBuffer[Index++] << 16) |
            (StatusBuffer[Index++] << 8) | StatusBuffer[Index++]);
    }

    CollimatorCurrentposition = StatusBuffer[Index++];
    CollimatorCurrentposition |= StatusBuffer[Index++] << 8;
    CollimatorCurrentposition |= StatusBuffer[Index++];
    CollimatorCurrentposition |= StatusBuffer[Index++] << 8;

    AckRecieved = StatusBuffer[Index++];
    if (AckRecieved == 1)
    {
        CommandBuffer[11] = 0;
    }
    EncoderCurrentPosCountsToDistance();
}



void Client::EncoderCurrentPosCountsToDistance()
{
//    CurrentEncoderPosition_Distance[(quint8)motor_select::Tube_lateral] =
//        ((DisplayCount1 * Protocol::LinearDistancePerRotation[(quint8)motor_select::Tube_lateral]) /
//        (Protocol::PulsePerRotation[(quint8)motor_select::Tube_lateral]));

//    CurrentEncoderPosition_Distance[(quint8)motor_select::Tube_longitudinal] =
//        ((DisplayCount2 * Protocol::LinearDistancePerRotation[(quint8)motor_select::Tube_longitudinal]) /
//        (Protocol::PulsePerRotation[(quint8)motor_select::Tube_longitudinal]));

    // Similar calculations for other motors...
}
