#ifndef SERVERCONTROLLER_H
#define SERVERCONTROLLER_H

#include <QObject>
#include "myserver.h"
#include "tcppollthread.h"

class ServerController : public QObject
{
    Q_OBJECT
public:
    explicit ServerController(QObject *parent = nullptr);
    Q_INVOKABLE void startServer();
    Q_INVOKABLE void sendToClient(int clientId, const QString &message);
    Q_INVOKABLE void broadcast(const QString &message);

signals:
    void newClientConnected(int clientId);
    void clientDisconnected(int clientId);
    void messageReceived(int clientId, const QString &message);

private slots:
    void onClientConnected(int clientId);
    void onClientDisconnected(int clientId);
    void onDataReceived(int clientId, const QByteArray &data);

private:
    MyServer *m_server;
    TcpPollThread* m_pollThread = nullptr;
};

#endif // SERVERCONTROLLER_H
