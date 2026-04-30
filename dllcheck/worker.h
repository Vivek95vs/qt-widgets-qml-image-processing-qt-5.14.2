#ifndef WORKER_H
#define WORKER_H

#include <QObject>
#include <QDebug>
#include <QMutex>

class Worker : public QObject
{
    Q_OBJECT

public:
    explicit Worker(QObject *parent = nullptr);
    void stopWork();  // Request the worker to stop

public slots:
    void doWork();    // Runs in a loop until stopped

signals:
    void newDataReceived(unsigned char value);  // Emits new data
    void workStopped();                        // Emits when stopped
    void errorOccurred(QString error);

private:
    bool m_running;   // Controls the loop
    QMutex m_mutex;   // Thread-safe flag access
};

#endif // WORKER_H
