#ifndef BOARDCONNECTION_H
#define BOARDCONNECTION_H

#include <QObject>
#include <QTcpSocket>
#include <QTimer>

class BoardConnection : public QObject
{
    Q_OBJECT
public:
    explicit BoardConnection(QObject *parent = nullptr);
    ~BoardConnection();

    bool isConnected() const;
    void connectToBoard(const QString &ipAddress, int port);
    void disconnectFromBoard();
    void sendData(const QByteArray &data);
    QByteArray sendAndReceive(const QByteArray &data, int timeoutMs = 1000);

signals:
    void connected();
    void disconnected();
    void errorOccurred(const QString &error);
    void dataReceived(const QByteArray &data);

private slots:
    void onConnected();
    void onDisconnected();
    void onError(QAbstractSocket::SocketError error);
    void onReadyRead();

private:
    QTcpSocket *m_socket;
    bool m_isConnected;
    QByteArray m_receiveBuffer;
    const int m_expectedDataSize = 500; // Adjust to match your protocol
};

#endif // BOARDCONNECTION_H
