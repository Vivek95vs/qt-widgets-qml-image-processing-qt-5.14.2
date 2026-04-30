#ifndef TCPPOLLTHREAD_H
#define TCPPOLLTHREAD_H

#include <QThread>
#include <QMutex>
#include "myserver.h"

class TcpPollThread : public QThread
{
    Q_OBJECT
public:
    explicit TcpPollThread(MyServer* server, QObject* parent = nullptr);
    ~TcpPollThread();

    void stop();
    void setPollInterval(int milliseconds);
    void setDataToSend(const QByteArray& data);
    void sendToClient(int clientId, const QByteArray& data);

protected:
    void run() override;

private:
    MyServer* m_server;
//    QByteArray m_data;
    int m_pollInterval;
    bool m_running;
    QMutex m_mutex;
};

#endif // TCPPOLLTHREAD_H
