#ifndef PROGRAMWORKER_H
#define PROGRAMWORKER_H

#include <QObject>
#include "Program.h"

class ProgramWorker : public QObject
{
    Q_OBJECT

public:
    explicit ProgramWorker(QObject *parent = nullptr);

public slots:
    void startProgram();

signals:
    void programStarted();
    void errorOccurred(const QString &error);
    void statusUpdate(const QString &status);
    void programFinished();

private:
    Program m_program;
};

#endif // PROGRAMWORKER_H
