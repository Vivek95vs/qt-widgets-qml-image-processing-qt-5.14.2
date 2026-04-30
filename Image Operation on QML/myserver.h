#ifndef MYSERVER_H
#define MYSERVER_H

#include <QTcpServer>
#include <QTcpSocket>
#include <QMap>

class MyServer : public QTcpServer
{
    Q_OBJECT

public:
    explicit MyServer(QObject *parent = nullptr);
    void startServer(quint16 port);
    void sendToClient(int clientId, const QByteArray &data);
    void broadcast(const QByteArray &data);
    QList<QTcpSocket*> getClients() const { return m_clients.values(); }

signals:
    void clientConnected(int clientId);
    void clientDisconnected(int clientId);
    void dataReceived(int clientId, const QByteArray &data);

protected:
    void incomingConnection(qintptr socketDescriptor) override;

private slots:
    void onReadyRead();
    void onDisconnected();

private:
    QMap<int, QTcpSocket*> m_clients;
    int m_nextClientId = 0;
};

#endif // MYSERVER_H
