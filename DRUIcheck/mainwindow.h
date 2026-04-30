#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QThread>
#include "Protocol.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class ProgramWorker;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_MotorEnable_clicked();
    void on_MotroAutoset_clicked();
    void on_Boardconnect_clicked();

    void handleProgramStarted();
    void handleProgramError(const QString &error);
    void handleStatusUpdate(const QString &status);
    void handleProgramFinished();

private:
    Ui::MainWindow *ui;
    ProgramWorker *m_programWorker;
    QThread *m_workerThread;
    Protocol *protol;
};

#endif // MAINWINDOW_H
