#ifndef MYSERVER_H
#define MYSERVER_H

#include <QTcpServer>
#include <QTcpSocket>
#include <QDebug>
#include "rnvexception.h"

class MyServer : public QTcpServer
{
    Q_OBJECT
public:
    explicit MyServer(QObject *parent = nullptr, quint16 port = 1234);
    ~MyServer();

    bool startServer();
    void writeData(const QByteArray &data);
    bool isSocketConnected() const;
    bool isStarted() const;

signals:
    void dataReceived(QByteArray data);
    void connectionStatusChanged(bool connected);

public slots:
    void readData();
    void onSocketStateChanged(QAbstractSocket::SocketState state);

protected:
    void incomingConnection(qintptr socketDescriptor) override;

private:
    QTcpSocket *m_socket;
    bool m_started;
    quint16 m_port;
};

#endif // MYSERVER_H
