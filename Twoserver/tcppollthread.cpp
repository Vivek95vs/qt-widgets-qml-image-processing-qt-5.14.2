#include "tcppollthread.h"
#include "myserver.h"

TcpPollThread::TcpPollThread(MyServer *server, QObject *parent) :
    QThread(parent),
    m_server(server),
    m_running(true)
{
}

void TcpPollThread::setPollData(const QByteArray &data)
{
    m_pollData = data;
}

void TcpPollThread::stop()
{
    m_running = false;
    quit();
    wait();
}
void TcpPollThread::updatePollData(const QByteArray &newData)
{
    QMutexLocker locker(&m_dataMutex);
    m_pollData = newData;
}
void TcpPollThread::run()
{
    while (m_running) {
        QByteArray dataToSend;
        {
            QMutexLocker locker(&m_dataMutex);
            dataToSend = m_pollData;
        }

        if (m_server && m_server->isSocketConnected()) {
            m_server->writeData(dataToSend);
        }
        msleep(1000); // Poll every second
    }
}
