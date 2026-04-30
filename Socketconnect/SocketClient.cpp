#include "SocketClient.h"
#include <QHostAddress>

SocketClient::SocketClient(QObject *parent) : QObject(parent),
    m_socket(new QTcpSocket(this)),
    m_ipAddress("127.0.0.1"),
    m_port(1234),
    m_connected(false)
{
    connect(m_socket, &QTcpSocket::connected, this, &SocketClient::onConnected);
    connect(m_socket, &QTcpSocket::disconnected, this, &SocketClient::onDisconnected);
    connect(m_socket, &QTcpSocket::readyRead, this, &SocketClient::onReadyRead);
//    connect(m_socket, QOverload<QAbstractSocket::SocketError>::of(&QTcpSocket::errorOccurred),
//            this, &SocketClient::onErrorOccurred);
}

void SocketClient::connectToServer()
{
    if (m_connected) {
        return;
    }

    m_socket->connectToHost(m_ipAddress, m_port);
    m_socket->waitForConnected(1000);
}

void SocketClient::disconnectFromServer()
{
    if (m_connected) {
        m_socket->disconnectFromHost();
    }
}

void SocketClient::sendData(const QByteArray &data)
{
    if (m_connected) {
        m_socket->write(data);
        m_socket->waitForBytesWritten(500);
    }
}

QByteArray SocketClient::receiveData()
{
    if (m_connected) {
        m_socket->waitForReadyRead(800);
        return m_socket->readAll();
    }
    return QByteArray();
}

void SocketClient::onConnected()
{
    m_connected = true;
    emit connectionChanged();
}

void SocketClient::onDisconnected()
{
    m_connected = false;
    emit connectionChanged();
}

void SocketClient::onReadyRead()
{
    m_receiveBuffer = m_socket->readAll();
    emit dataReceived(m_receiveBuffer);
}

//void SocketClient::onErrorOccurred(QAbstractSocket::SocketError error)
//{
//    m_connected = false;
//    emit connectionChanged();
//    emit errorOccurred(m_socket->errorString());
//}

// Property getters/setters
QString SocketClient::ipAddress() const { return m_ipAddress; }
void SocketClient::setIpAddress(const QString &ipAddress) { m_ipAddress = ipAddress; emit ipAddressChanged(); }
int SocketClient::port() const { return m_port; }
void SocketClient::setPort(int port) { m_port = port; emit portChanged(); }
bool SocketClient::connected() const { return m_connected; }
