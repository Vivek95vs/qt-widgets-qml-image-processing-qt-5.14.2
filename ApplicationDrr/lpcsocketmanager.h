#ifndef LPCSOCKETMANAGER_H
#define LPCSOCKETMANAGER_H

#include <QObject>
#include <QTcpSocket>

class LPCSocketManager : public QObject
{
    Q_OBJECT

public:
    explicit LPCSocketManager(QObject *parent = nullptr);
    ~LPCSocketManager();

    void connectToServer(const QString &ipAddress, int portNumber);
    void disconnectFromServer();

    bool isConnected() const;
    int getCommunicationStatus() const;
    bool getEventTxRxTrigger() const;

signals:
    void connected();
    void disconnected();
    void errorOccurred(const QString &error);

private:
    QTcpSocket *clientSocket;
    QByteArray rx_DR_data;

    QString IP_Address;
    int PortNumber;

    bool ConnectStatus = true;
    int Communication_establish = 0;
    bool eventTxRxTrigger = false;
    int ReconnectFlag = 0;
};

#endif // LPCSOCKETMANAGER_H
