#include "ProgramWorker.h"

ProgramWorker::ProgramWorker(QObject *parent) : QObject(parent)
{
}

void ProgramWorker::startProgram()
{
    try {
        emit statusUpdate("Starting program...");
        m_program.Main();
        emit programStarted();
    } catch (const std::exception &e) {
        emit errorOccurred(QString("Error: ") + e.what());
    } catch (...) {
        emit errorOccurred("Unknown error occurred");
    }
    emit programFinished();
}
