#include "worker.h"
//#include "ClrWrapper.h"
#include "QThread"

Worker::Worker(QObject *parent) : QObject(parent), m_running(false) {}

void Worker::doWork()
{
//    m_mutex.lock();
//    m_running = true;
//    m_mutex.unlock();

//    try {
//        while (true) {
//            m_mutex.lock();
//            if (!m_running) break;  // Exit if stopped
//            m_mutex.unlock();

//            MainMethod();

//            // Fetch data from .NET
//            for (int i = 0; i < 500; i++) {
//                unsigned char value = GetByteValue(i);
//                emit newDataReceived(value);
//                QThread::msleep(10);  // Small delay (adjust as needed)
//            }
//        }
//    } catch (...) {
//        emit errorOccurred("Failed in .NET interop");
//    }

//    emit workStopped();  // Signal that work has stopped
}

void Worker::stopWork()
{
    QMutexLocker locker(&m_mutex);
    m_running = false;  // Safely stop the loop
}
