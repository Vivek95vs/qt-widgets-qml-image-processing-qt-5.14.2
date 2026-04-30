#include "myserver.h"

MyServer::MyServer(QObject *parent, quint16 port) :
    QTcpServer(parent),
    m_socket(nullptr),
    m_started(false),
    m_port(port)
{
}

MyServer::~MyServer()
{
    if (m_socket) {
        m_socket->disconnectFromHost();
        if (m_socket->state() != QAbstractSocket::UnconnectedState) {
            m_socket->waitForDisconnected();
        }
        delete m_socket;
    }
}

bool MyServer::startServer()
{
    if (isListening()) {
        return true;
    }

    if (!listen(QHostAddress::Any, m_port)) {
        qCritical() << "Failed to start server on port" << m_port << ":" << errorString();
        throw RnVException("Unable to start server on port " + QString::number(m_port));
        return false;
    }

    m_started = true;
    qInfo() << "Server started on port" << m_port;
    return true;
}

void MyServer::writeData(const QByteArray &data)
{
    if (m_socket && m_socket->state() == QAbstractSocket::ConnectedState) {
        m_socket->write(data);
        if (!m_socket->waitForBytesWritten(3000)) {
            qWarning() << "Failed to write data to socket on port" << m_port;
        }
    }
}

bool MyServer::isSocketConnected() const
{
    return m_socket && m_socket->state() == QAbstractSocket::ConnectedState;
}

bool MyServer::isStarted() const
{
    return m_started;
}

void MyServer::readData()
{
    if (!m_socket) return;

    QByteArray data = m_socket->readAll();
    if (!data.isEmpty()) {
        emit dataReceived(data);
    }
}

void MyServer::onSocketStateChanged(QAbstractSocket::SocketState state)
{
    qDebug() << "Socket state changed on port" << m_port << ":" << state;

    bool connected = (state == QAbstractSocket::ConnectedState);
    emit connectionStatusChanged(connected);

    if (state == QAbstractSocket::UnconnectedState) {
        m_socket->deleteLater();
        m_socket = nullptr;
    }
}

void MyServer::incomingConnection(qintptr socketDescriptor)
{
    if (m_socket) {
        qWarning() << "Rejecting new connection on port" << m_port << "- already has a client";
        QTcpSocket tempSocket;
        tempSocket.setSocketDescriptor(socketDescriptor);
        tempSocket.disconnectFromHost();
        return;
    }

    m_socket = new QTcpSocket(this);
    if (!m_socket->setSocketDescriptor(socketDescriptor)) {
        qCritical() << "Failed to set socket descriptor on port" << m_port;
        delete m_socket;
        m_socket = nullptr;
        return;
    }

    connect(m_socket, &QTcpSocket::readyRead, this, &MyServer::readData);
    connect(m_socket, &QTcpSocket::stateChanged, this, &MyServer::onSocketStateChanged);
    m_socket->setSocketOption(QAbstractSocket::KeepAliveOption, 1);

    qInfo() << "New client connected on port" << m_port;
    emit connectionStatusChanged(true);
}
