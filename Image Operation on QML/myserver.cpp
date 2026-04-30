#include "myserver.h"
#include <QDebug>

MyServer::MyServer(QObject *parent) : QTcpServer(parent) {}

void MyServer::startServer(quint16 port)
{
    if(!this->listen(QHostAddress::Any, port)) {
        qDebug() << "Server could not start!";
    } else {
        qDebug() << "Server started on port" << port;
    }
}

void MyServer::incomingConnection(qintptr socketDescriptor)
{
    QTcpSocket *socket = new QTcpSocket(this);
    if(!socket->setSocketDescriptor(socketDescriptor)) {
        qDebug() << "Could not set socket descriptor";
        return;
    }

    int clientId = m_nextClientId++;
    socket->setProperty("clientId", clientId);
    m_clients.insert(clientId, socket);

    connect(socket, &QTcpSocket::readyRead, this, &MyServer::onReadyRead);
    connect(socket, &QTcpSocket::disconnected, this, &MyServer::onDisconnected);

    qDebug() << "Client connected with ID:" << clientId;
    emit clientConnected(clientId);
}

void MyServer::onReadyRead()
{
    QTcpSocket *socket = qobject_cast<QTcpSocket*>(sender());
    if(!socket) return;

    int clientId = m_clients.key(socket);
    QByteArray data = socket->readAll();

    qDebug() << "Received from client" << clientId << ":" << data;
    emit dataReceived(clientId, data);
}

void MyServer::onDisconnected()
{
    QTcpSocket *socket = qobject_cast<QTcpSocket*>(sender());
    if(!socket) return;

    int clientId = m_clients.key(socket);
    m_clients.remove(clientId);

    qDebug() << "Client disconnected:" << clientId;
    emit clientDisconnected(clientId);

    socket->deleteLater();
}

void MyServer::sendToClient(int clientId, const QByteArray &data)
{
    if(m_clients.contains(clientId)) {
        QTcpSocket *socket = m_clients.value(clientId);
        if(socket->state() == QTcpSocket::ConnectedState) {
            socket->write(data);
            socket->flush();
            qDebug() << "Sent to client" << clientId << ":" << data;
        }
    }
}

void MyServer::broadcast(const QByteArray &data)
{
    for(QTcpSocket *socket : m_clients) {
        if(socket->state() == QTcpSocket::ConnectedState) {
            socket->write(data);
            socket->flush();
        }
    }
    qDebug() << "Broadcasted to all clients:" << data;
}
