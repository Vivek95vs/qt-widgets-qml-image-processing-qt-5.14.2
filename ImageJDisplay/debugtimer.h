#ifndef DEBUGTIMER_H
#define DEBUGTIMER_H


#pragma once

#include <QElapsedTimer>
#include <QDebug>

class DebugTimer
{
public:
    explicit DebugTimer(const char *functionName)
        : m_functionName(functionName)
    {
        m_timer.start();

        qDebug().noquote()
            << "[ENTER]"
            << m_functionName;
    }

    ~DebugTimer()
    {
        qDebug().noquote()
            << "[EXIT ]"
            << m_functionName
            << "| Time:"
            << m_timer.elapsed()
            << "ms";
    }

private:
    const char *m_functionName;
    QElapsedTimer m_timer;
};



#endif // DEBUGTIMER_H
