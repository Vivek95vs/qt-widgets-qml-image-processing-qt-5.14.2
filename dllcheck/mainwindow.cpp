#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "CppWrapper.h"
#include "worker.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
      ui(new Ui::MainWindow),
      worker(new Worker()),
      workerThread(new QThread(this)),
      m_workerRunning(false)
{
//    ui->setupUi(this);
//    worker->moveToThread(workerThread);

//    // Connect worker signals
//    connect(worker, &Worker::newDataReceived, this, &MainWindow::handleNewData);
//    connect(worker, &Worker::workStopped, this, &MainWindow::handleWorkStopped);
//    connect(worker, &Worker::errorOccurred, this, &MainWindow::handleError);

//    // Start thread (but worker remains idle until started)
//    workerThread->start();
}

MainWindow::~MainWindow()
{
    if (m_workerRunning) {
        worker->stopWork();
        workerThread->quit();
        workerThread->wait();
    }
    delete ui;
}

void MainWindow::handleNewData(unsigned char value)
{
    // Update UI (e.g., append to a log or plot)
//    ui->textEdit->append(QString("Value: %1").arg(value));
}

void MainWindow::handleWorkStopped()
{
    // Optional: Cleanup or reset UI
    qDebug() << "Worker stopped safely";
}

void MainWindow::handleError(QString error)
{
//    ui->statusLabel->setText("Error: " + error);
    m_workerRunning = false;
}

void MainWindow::on_start_clicked()
{
    if (!m_workerRunning) {
        QMetaObject::invokeMethod(worker, "doWork", Qt::QueuedConnection);
        m_workerRunning = true;
//        ui->statusLabel->setText("Running...");
    }
}

void MainWindow::on_stop_clicked()
{
    if (m_workerRunning) {
        worker->stopWork();  // Triggers loop exit
        m_workerRunning = false;
//        ui->statusLabel->setText("Stopped");
    }
}

void MainWindow::on_pushButton_clicked()
{
//    MainMethod();

//    // Fetch data from .NET
//    for (int i = 0; i < 10; i++) {
//        unsigned char value = GetByteValue(i);
////        emit newDataReceived(value);
//        QThread::msleep(10);  // Small delay (adjust as needed)
//        qDebug()<<"Value"<<value;
//    }

    RunMain();
    MotorEnableFunction(3);
}
