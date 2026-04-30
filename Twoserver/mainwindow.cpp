#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>

QByteArray  TCPData="0";
QByteArray  TCPData1="0";
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , m_server1(this, 1234)  // First server on port 1234
    , m_server2(this, 1235)  // Second server on port 1235
    , m_pollThread1(nullptr)
    , m_pollThread2(nullptr)
{
    ui->setupUi(this);
    startServers();
}

MainWindow::~MainWindow()
{
    if (m_pollThread1) {
        m_pollThread1->stop();
        delete m_pollThread1;
    }
    if (m_pollThread2) {
        m_pollThread2->stop();
        delete m_pollThread2;
    }
    delete ui;
}

void MainWindow::startServers()
{
    // Start Server 1
    if (!m_server1.isStarted()) {
        try {
            if (m_server1.startServer()) {
                connect(&m_server1, &MyServer::dataReceived,
                        this, &MainWindow::onDataReceivedFromServer1);
                connect(&m_server1, &MyServer::connectionStatusChanged,
                        this, &MainWindow::onServer1ConnectionChanged);

                m_pollThread1 = new TcpPollThread(&m_server1);
                m_pollThread1->setPollData(TCPData);
                m_pollThread1->start();
            }
        } catch (const RnVException &e) {
//            QMessageBox::critical(this, "Server Error",
//                                 "Failed to start Server 1: " + e.message());
        }
    }

    // Start Server 2
    if (!m_server2.isStarted()) {
        try {
            if (m_server2.startServer()) {
                connect(&m_server2, &MyServer::dataReceived,
                        this, &MainWindow::onDataReceivedFromServer2);
                connect(&m_server2, &MyServer::connectionStatusChanged,
                        this, &MainWindow::onServer2ConnectionChanged);

                m_pollThread2 = new TcpPollThread(&m_server2);
                m_pollThread2->setPollData(TCPData1);
                m_pollThread2->start();
            }
        } catch (const RnVException &e) {
//            QMessageBox::critical(this, "Server Error",
//                                 "Failed to start Server 2: " + e.message());
        }
    }
}

void MainWindow::onDataReceivedFromServer1(QByteArray data)
{
    qDebug() << "Server 1 received:" << data;
    // Update UI or process data from server 1's client

    QString message = QString::fromUtf8(data);



}

void MainWindow::onDataReceivedFromServer2(QByteArray data)
{
    qDebug() << "Server 2 received:" << data;
    // Update UI or process data from server 2's client
}

void MainWindow::onServer1ConnectionChanged(bool connected)
{
    qDebug() << "Server 1 connection:" << (connected ? "Connected" : "Disconnected");
    // Update UI for server 1 connection status
}

void MainWindow::onServer2ConnectionChanged(bool connected)
{
    qDebug() << "Server 2 connection:" << (connected ? "Connected" : "Disconnected");
    // Update UI for server 2 connection status
}

// Send data to Server 1's client
void MainWindow::sendToClient1(const QByteArray &data)
{
    if (m_server1.isSocketConnected()) {
        m_server1.writeData(data);
    } else {
        qWarning() << "Cannot send data - Server 1 client not connected";
    }
}

// Send data to Server 2's client
void MainWindow::sendToClient2(const QByteArray &data)
{
    if (m_server2.isSocketConnected()) {
        m_server2.writeData(data);
    } else {
        qWarning() << "Cannot send data - Server 2 client not connected";
    }
}

void MainWindow::on_server1_clicked()
{
    QString message = ui->lineEdit->text();
    TCPData=message.toUtf8();
    sendToClient1(TCPData);

    // Update the polling thread with new data
    if (m_pollThread1) {
        m_pollThread1->updatePollData(TCPData);
    }
}

void MainWindow::on_server2_clicked()
{
    QString message = ui->lineEdit_2->text();
    TCPData1=message.toUtf8();
    sendToClient2(TCPData1);

    if (m_pollThread2) {
        m_pollThread2->updatePollData(TCPData1);
    }
}
