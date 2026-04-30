#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "ProgramWorker.h"
#include "QDebug"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , m_programWorker(nullptr)
    , m_workerThread(nullptr)
{
    ui->setupUi(this);
    protol=new Protocol(this);
}

MainWindow::~MainWindow()
{
    if (m_workerThread) {
        m_workerThread->quit();
        m_workerThread->wait();
        delete m_workerThread;
    }
    delete ui;
}

void MainWindow::on_MotorEnable_clicked()
{

    quint8 Motornum_Enable = 1;

    // Assuming you have checkboxes named tubeLateralCheckBox, etc.
//    if (ui->tubeLateralCheckBox->isChecked()) {
//        Motornum_Enable |= 1 << 0;
//    }
//    // Add similar checks for other motors...

//    if (Motornum_Enable != 0) {
        protol->motorEnableFunction(Motornum_Enable);
//    }
}

void MainWindow::on_MotroAutoset_clicked()
{
//    ui->autoStartButton->setStyleSheet("background-color: gray;");
    try {
        protol->Motor_Select = 0;

//        if (ui->tubeLateralCheckBox->isChecked()) {
            protol->Motor_Select |= 1 << 0;
            protol->TargetPosition[static_cast<quint8>(Client::motor_select::Tube_lateral)-1] = ui->Position->text().toDouble();
            protol->velocity[static_cast<quint8>(Client::motor_select::Tube_lateral)-1] = ui->Velocity->text().toInt();
//        }
        // Add similar checks for other motors...

//        if (Protocol::Motor_Select != 0) {
            protol->servoAutoStart(protol->Motor_Select,
                                     protol->velocity,
                                     protol->TargetPosition);
//        }
    } catch (...) {
        qDebug() << "Error in auto start";
    }
}

void MainWindow::on_Boardconnect_clicked()
{
    if (m_workerThread && m_workerThread->isRunning()) {
        qDebug() << "Program is already running";
        return;
    }

    // Create new thread and worker
    m_workerThread = new QThread(this);
    m_programWorker = new ProgramWorker();

    // Move worker to thread
    m_programWorker->moveToThread(m_workerThread);

    // Connect signals
    connect(m_workerThread, &QThread::started, m_programWorker, &ProgramWorker::startProgram);
    connect(m_workerThread, &QThread::finished, m_programWorker, &QObject::deleteLater);

    connect(m_programWorker, &ProgramWorker::programStarted, this, &MainWindow::handleProgramStarted);
    connect(m_programWorker, &ProgramWorker::errorOccurred, this, &MainWindow::handleProgramError);
    connect(m_programWorker, &ProgramWorker::statusUpdate, this, &MainWindow::handleStatusUpdate);
    connect(m_programWorker, &ProgramWorker::programFinished, this, &MainWindow::handleProgramFinished);

    // Start the thread
    m_workerThread->start();
}

void MainWindow::handleProgramStarted()
{
    ui->statusbar->showMessage("Program started successfully");
}

void MainWindow::handleProgramError(const QString &error)
{
    ui->statusbar->showMessage(error);
    if (m_workerThread) {
        m_workerThread->quit();
    }
}

void MainWindow::handleStatusUpdate(const QString &status)
{
    ui->statusbar->showMessage(status, 3000); // Show for 3 seconds
}

void MainWindow::handleProgramFinished()
{
    ui->statusbar->showMessage("Program finished");
    if (m_workerThread) {
        m_workerThread->quit();
    }
}

// Other button handlers...
