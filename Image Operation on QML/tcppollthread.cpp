#include "tcppollthread.h"
#include <QDebug>
QByteArray  TCPData="0";
TcpPollThread::TcpPollThread(MyServer* server, QObject* parent)
    : QThread(parent),
      m_server(server),
      m_pollInterval(1000), // Default 1 second
      m_running(false)
{
}

TcpPollThread::~TcpPollThread()
{
    stop();
    wait(); // Wait for thread to finish
}

void TcpPollThread::stop()
{
    QMutexLocker locker(&m_mutex);
    m_running = false;
}

void TcpPollThread::sendToClient(int clientId, const QByteArray& data)
{
    QMutexLocker locker(&m_mutex);
    if (m_server) {
        QTcpSocket* client = nullptr;
        // Find the client with matching ID
        const auto clients = m_server->getClients();
        for (auto* socket : clients) {
            if (socket->property("clientId").toInt() == clientId) {
                client = socket;
                break;
            }
        }

        if (client && client->state() == QTcpSocket::ConnectedState) {
            client->write(data);
            client->flush();
            qDebug() << "Sent data to client:" << clientId;
        }
    }
}

void TcpPollThread::setPollInterval(int milliseconds)
{
    QMutexLocker locker(&m_mutex);
    m_pollInterval = milliseconds;
}

void TcpPollThread::setDataToSend(const QByteArray& data)
{
    QMutexLocker locker(&m_mutex);
    TCPData = data;
}

void TcpPollThread::run()
{
    {
        QMutexLocker locker(&m_mutex);
        m_running = true;
    }

    qDebug() << "Polling thread started";

    while (true) {
        {
            QMutexLocker locker(&m_mutex);
            if (!m_running) break;

            if (m_server && !TCPData.isEmpty()) {
                // Parse the data to get client ID
                QList<QByteArray> parts = TCPData.split(',');
                if (parts.size() >= 2) {
                    int clientId = parts[0].toInt();
                    QByteArray actualData = parts[1];

                    // Find the specific client
                    const auto clients = m_server->getClients();
                    for (auto* client : clients) {
                        if (client->property("clientId").toInt() == clientId) {
                            if (client->state() == QTcpSocket::ConnectedState) {
                                client->write(actualData);
                                client->flush();
                                qDebug() << "Sent data to client:" << clientId;
                            }
                            break;
                        }
                    }
                }
                TCPData.clear(); // Clear after sending
            }
        }
        msleep(m_pollInterval);
    }
    qDebug() << "Polling thread stopped";
}
