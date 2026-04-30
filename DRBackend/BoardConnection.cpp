#include "BoardConnection.h"
#include <QDebug>

BoardConnection::BoardConnection(QObject *parent) : QObject(parent),
    m_socket(new QTcpSocket(this)),
    m_isConnected(false)
{
    m_socket->setSocketOption(QAbstractSocket::LowDelayOption, 1);
    m_socket->setSocketOption(QAbstractSocket::KeepAliveOption, 1);

    connect(m_socket, &QTcpSocket::connected, this, &BoardConnection::onConnected);
    connect(m_socket, &QTcpSocket::disconnected, this, &BoardConnection::onDisconnected);
    connect(m_socket, QOverload<QAbstractSocket::SocketError>::of(&QTcpSocket::error),
            this, &BoardConnection::onError);
    connect(m_socket, &QTcpSocket::readyRead, this, &BoardConnection::onReadyRead);
}

BoardConnection::~BoardConnection()
{
    disconnectFromBoard();
}

bool BoardConnection::isConnected() const
{
    return m_isConnected;
}

void BoardConnection::connectToBoard(const QString &ipAddress, int port)
{
    if (m_isConnected) {
        disconnectFromBoard();
    }

    qDebug() << "Connecting to" << ipAddress << ":" << port;
    m_socket->connectToHost(ipAddress, port);
}

void BoardConnection::disconnectFromBoard()
{
    if (m_socket->state() != QAbstractSocket::UnconnectedState) {
        m_socket->disconnectFromHost();
        if (m_socket->state() != QAbstractSocket::UnconnectedState) {
            m_socket->waitForDisconnected(1000);
        }
    }
    m_isConnected = false;
}

void BoardConnection::sendData(const QByteArray &data)
{
    if (!m_isConnected) {
        qWarning() << "Attempted to send data while disconnected";
        return;
    }

    qDebug() << "Sending data:" << data.toHex();
    m_socket->write(data);

    if (!m_socket->waitForBytesWritten(1000)) {
        qWarning() << "Failed to write data:" << m_socket->errorString();
        m_isConnected = false;
        emit errorOccurred("Write timeout: " + m_socket->errorString());
        emit disconnected();
    }
}

QByteArray BoardConnection::sendAndReceive(const QByteArray &data, int timeoutMs)
{
    if (!m_isConnected) {
        qWarning() << "Not connected to board";
        return QByteArray();
    }

    // Clear any pending data
    m_socket->readAll();

    // Send command
    qDebug() << "Sending command:" << data.toHex();
    m_socket->write(data);
    if (!m_socket->waitForBytesWritten(timeoutMs)) {
        qWarning() << "Write timeout:" << m_socket->errorString();
        return QByteArray();
    }

    // Wait for response
    if (!m_socket->waitForReadyRead(timeoutMs)) {
        qWarning() << "Read timeout:" << m_socket->errorString();
        return QByteArray();
    }

    // Read response
    QByteArray response = m_socket->readAll();
    qDebug() << "Received response:" << response.toHex();
    return response;
}

void BoardConnection::onConnected()
{
    m_isConnected = true;
    qDebug() << "Successfully connected to board";
    emit connected();
}

void BoardConnection::onDisconnected()
{
    m_isConnected = false;
    emit disconnected();
}

void BoardConnection::onError(QAbstractSocket::SocketError error)
{
    qDebug() << "Socket error:" << error << m_socket->errorString();
    m_isConnected = false;
    emit errorOccurred(m_socket->errorString());
    emit disconnected(); // Ensure disconnected signal is emitted on error
}

void BoardConnection::onReadyRead()
{
    while (m_socket->bytesAvailable() > 0) {
        m_receiveBuffer.append(m_socket->readAll());

//        qDebug() << "Current buffer size:" << m_receiveBuffer.size() << "bytes";
//        qDebug() << "Buffer content:" << m_receiveBuffer.toHex();

        // Check if we have a complete packet
        if (m_receiveBuffer.size() >= m_expectedDataSize) {
            QByteArray completePacket = m_receiveBuffer.left(m_expectedDataSize);
            m_receiveBuffer.remove(0, m_expectedDataSize);

            qDebug() << "Complete packet received (" << completePacket.size() << "bytes):";
            qDebug() << completePacket.toHex();

            emit dataReceived(completePacket);
        }
    }
}
