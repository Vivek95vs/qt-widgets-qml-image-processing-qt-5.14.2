#ifndef SOCKETCLIENT_H
#define SOCKETCLIENT_H

#include <QObject>
#include <QTcpSocket>
#include <QTimer>

class SocketClient : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString ipAddress READ ipAddress WRITE setIpAddress NOTIFY ipAddressChanged)
    Q_PROPERTY(int port READ port WRITE setPort NOTIFY portChanged)
    Q_PROPERTY(bool connected READ connected NOTIFY connectionChanged)

public:
    explicit SocketClient(QObject *parent = nullptr);

    QString ipAddress() const;
    void setIpAddress(const QString &ipAddress);

    int port() const;
    void setPort(int port);

    bool connected() const;

    Q_INVOKABLE void connectToServer();
    Q_INVOKABLE void disconnectFromServer();
    Q_INVOKABLE void sendData(const QByteArray &data);
    Q_INVOKABLE QByteArray receiveData();

signals:
    void ipAddressChanged();
    void portChanged();
    void connectionChanged();
    void dataReceived(const QByteArray &data);
    void errorOccurred(const QString &error);

private slots:
    void onConnected();
    void onDisconnected();
    void onReadyRead();
//    void onErrorOccurred(QAbstractSocket::SocketError error);

private:
    QTcpSocket *m_socket;
    QString m_ipAddress;
    int m_port;
    bool m_connected;
    QByteArray m_receiveBuffer;
};

#endif // SOCKETCLIENT_H
