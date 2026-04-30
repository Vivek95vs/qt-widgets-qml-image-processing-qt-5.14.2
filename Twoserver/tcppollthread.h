#ifndef TCPPOLLTHREAD_H
#define TCPPOLLTHREAD_H

#include <QThread>
#include <QByteArray>
#include "QMutex"

class MyServer;

class TcpPollThread : public QThread
{
    Q_OBJECT
public:
    explicit TcpPollThread(MyServer *server, QObject *parent = nullptr);
    void setPollData(const QByteArray &data);
    void updatePollData(const QByteArray &newData);  // Add this new method
    void stop();

protected:
    void run() override;

private:
    MyServer *m_server;
    QByteArray m_pollData;
    bool m_running;
    QMutex m_dataMutex;
};

#endif // TCPPOLLTHREAD_H
