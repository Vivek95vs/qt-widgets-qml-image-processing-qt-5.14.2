#include "servercontroller.h"
#include <QDebug>

ServerController::ServerController(QObject *parent) : QObject(parent)
{
    m_server = new MyServer(this);

    connect(m_server, &MyServer::clientConnected,
            this, &ServerController::onClientConnected);
    connect(m_server, &MyServer::clientDisconnected,
            this, &ServerController::onClientDisconnected);
    connect(m_server, &MyServer::dataReceived,
            this, &ServerController::onDataReceived);
}

void ServerController::startServer()
{
    qDebug() << "Attempting to start TCP Server...";

    if (!m_server->isListening()) {
        const quint16 port = 1234;
        m_server->startServer(port);
        qDebug() << "TCP Server started on port" << port;

        // Initialize and start polling thread
        m_pollThread = new TcpPollThread(m_server, this);
        m_pollThread->setPollInterval(1000); // 1 second interval
        m_pollThread->start();

        connect(m_server, &MyServer::clientConnected, this, [this](int clientId) {
            qDebug() << "Client connected:" << clientId;
            emit newClientConnected(clientId);
        });

        // ... rest of your connections
    }
}

void ServerController::sendToClient(int clientId, const QString &message)
{
    m_server->sendToClient(clientId, message.toUtf8());
}

void ServerController::broadcast(const QString &message)
{
    m_server->broadcast(message.toUtf8());
}

void ServerController::onClientConnected(int clientId)
{
    qDebug() << "Controller: Client connected:" << clientId;
    emit newClientConnected(clientId);
}

void ServerController::onClientDisconnected(int clientId)
{
    qDebug() << "Controller: Client disconnected:" << clientId;
    emit clientDisconnected(clientId);
}

void ServerController::onDataReceived(int clientId, const QByteArray &data)
{
    QString message = QString::fromUtf8(data);
    qDebug() << "Controller: Message from client" << clientId << ":" << message;
    emit messageReceived(clientId, message);
}
