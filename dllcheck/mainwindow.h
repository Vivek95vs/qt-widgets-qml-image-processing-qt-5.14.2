#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QThread>

namespace Ui {
class MainWindow;
}

class Worker;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void handleNewData(unsigned char value);  // Updates UI with new data
    void handleWorkStopped();         // Cleans up after stop
    void handleError(QString error);  // Handles errors

    void on_start_clicked();

    void on_stop_clicked();

    void on_pushButton_clicked();

private:
    Ui::MainWindow *ui;
    Worker *worker;
    QThread *workerThread;
    bool m_workerRunning;  // Tracks worker state
};

#endif // MAINWINDOW_H
