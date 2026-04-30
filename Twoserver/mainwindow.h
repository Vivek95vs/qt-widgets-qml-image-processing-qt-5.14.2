#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "myserver.h"
#include "tcppollthread.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void startServers();
    void onDataReceivedFromServer1(QByteArray data);
    void onDataReceivedFromServer2(QByteArray data);
    void onServer1ConnectionChanged(bool connected);
    void onServer2ConnectionChanged(bool connected);
    void sendToClient1(const QByteArray &data);
    void sendToClient2(const QByteArray &data);

    void on_server1_clicked();

    void on_server2_clicked();

private:
    Ui::MainWindow *ui;
    MyServer m_server1;
    MyServer m_server2;
    TcpPollThread *m_pollThread1;
    TcpPollThread *m_pollThread2;
};
#endif // MAINWINDOW_H
