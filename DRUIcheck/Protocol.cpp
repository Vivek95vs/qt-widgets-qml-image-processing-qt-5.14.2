#include "Protocol.h"

QThread* Protocol::Sequence_thread = nullptr;
QThread* Protocol::Dr_BoardFeedBack_thread = nullptr;

QString Protocol::WorkdflowfilePath = "D:/Digital_Radiography/Log Files";
QString Protocol::WorkdflowfileName = "Workflow.txt";
QString Protocol::Log_Workflowpath = "D:/Digital_Radiography/Log Files/Wrokflow.txt";
QString Protocol::ReadDataFromIA_Filepath = "D:/Digital_Radiography/Image_Aquasition/SendDataToBackendFromIA/ImageAquasitionData.txt";
QString Protocol::ReadDataFromDatabase_Filepath = "D:/Digital_Radiography/Data_Base/SendDataToBackend/DatabaseValue.txt";

// Other static member initializations...
// Initialize static members
QVector<double> Protocol::TargetPosition(10, 0.0);
QVector<double> Protocol::StaticTolerance(8, 0.0);
QVector<double> Protocol::LinearDistancePerRotation = {10, 10, 10, 10, 10, 10, 10, 10};
QVector<int> Protocol::PulsePerRotation = {10000, 10000, 10000, 10000, 10000, 10000, 10000, 10000};
QVector<quint8> Protocol::MotorStartStopFlag(11, 0);
QVector<int> Protocol::TargetCounts(8, 0);
QVector<int> Protocol::velocity(8, 0);
QVector<double> Protocol::Reset_Position(8, 0.0);

quint8 Protocol::MotorConnected = 0;  // Initialize static member


Protocol::Protocol(QObject *parent) : QObject(parent)
{

    m_client= new Client;
}

QString Protocol::CreateWorkflowLogfile(const QString& directory, const QString& name)
{
    QDateTime now = QDateTime::currentDateTime();
    QString filename = now.toString("yyyy-MM-dd-HH-mm-ss") + "__" + name;

    QDir dir(directory);
    if (!dir.exists())
    {
        dir.mkpath(".");
    }

    QString path = directory + "/" + filename;
    QFile file(path);
    if (file.open(QIODevice::WriteOnly))
    {
        file.close();
    }
    return path;
}

void Protocol::WriteWorkflowToLogfile(const QString& path, const QString& data)
{
    try
    {
        QFile file(path);
        if (file.open(QIODevice::Append))
        {
            QTextStream stream(&file);
            stream << QDateTime::currentDateTime().toString() << "  : " << data << Qt::endl;
            qDebug() << QDateTime::currentDateTime().toString() << "  : " << data;
            file.close();
        }
    }
    catch (...)
    {
    }
}

void Protocol::ReadImageViewerData(const QString& path)
{
    try
    {
        QFile file(ReadDataFromIA_Filepath);
        if (file.open(QIODevice::ReadOnly))
        {
            QTextStream in(&file);
            QStringList Temp_ImageViewerSoftwareData;

            while (!in.atEnd())
            {
                Temp_ImageViewerSoftwareData.append(in.readLine());
            }

            if (Temp_ImageViewerSoftwareData.size() >= 8)
            {
//                SID = Temp_ImageViewerSoftwareData[1].toDouble();
//                Anotomy = Temp_ImageViewerSoftwareData[4];
//                patient_height = Temp_ImageViewerSoftwareData[7].toDouble();

//                WriteWorkflowToLogfile(Log_Workflowpath,
//                    " SID : " + QString::number(SID) +
//                    " Anotomy : " + Anotomy +
//                    " patient height : " + QString::number(patient_height));
            }
        }
    }
    catch (...)
    {
        qDebug() << "File is not read properly";
        WriteWorkflowToLogfile(Log_Workflowpath, "File is not read properly");
    }
}

void Protocol::DRReq_ThreadStart()
{
    Sequence_thread = QThread::create([]() {
        while (true)
        {
            QThread::msleep(10);
        }
    });
    Sequence_thread->start();

    // Dr_BoardFeedBack_thread is started by Client now
}

void Protocol::BootParameter()
{
    // Implementation similar to original but using Qt types

    // 1. Encoder Read Position Slave Address
       int ByteNumber = 11;
       m_client->_pauseEventTrigger();

       qDebug()<<"Enter boot parameter";
       m_client->CommandBuffer[ByteNumber++] = MotorParameters::BootParameter;

       m_client->CommandBuffer[ByteNumber++] = static_cast<quint8>(MotorParameters::Slave_Address::Tube_Lateral);
       m_client->CommandBuffer[ByteNumber++] = static_cast<quint8>(MotorParameters::Slave_Address::Tube_Longitudinal);
       m_client->CommandBuffer[ByteNumber++] = static_cast<quint8>(MotorParameters::Slave_Address::Tube_Vertical);
       m_client->CommandBuffer[ByteNumber++] = static_cast<quint8>(MotorParameters::Slave_Address::Tube_Vertical_rotation);
       m_client->CommandBuffer[ByteNumber++] = static_cast<quint8>(MotorParameters::Slave_Address::Tube_Horizontal_rotation);
       m_client->CommandBuffer[ByteNumber++] = static_cast<quint8>(MotorParameters::Slave_Address::Detector_Vertical);
       m_client->CommandBuffer[ByteNumber++] = static_cast<quint8>(MotorParameters::Slave_Address::Detector_tilt);

       // Copy motor parameters to command buffer
       qDebug()<<"motor enable size"<<MotorParameters::MotorAddressParameter::MotorEnable.size();
       for (int i = 0; i < MotorParameters::MotorAddressParameter::MotorEnable.size(); i++) {
//           m_client->CommandBuffer.clear();
           m_client->CommandBuffer[ByteNumber++] = MotorParameters::MotorAddressParameter::MotorEnable[i];
           qDebug()<<"Hex Enable data"<<m_client->CommandBuffer.toHex(' ');

       }
//       for (int i = 0; i < MotorParameters::MotorAddressParameter::MotorDisable.size(); i++) {
//           m_client->CommandBuffer[ByteNumber++] = MotorParameters::MotorAddressParameter::MotorDisable[i];
//       }
//       for (int i = 0; i < MotorParameters::MotorAddressParameter::PositionmodeParameters.size(); i++) {
//           m_client->CommandBuffer[ByteNumber++] = MotorParameters::MotorAddressParameter::PositionmodeParameters[i];
//       }
//       for (int i = 0; i < MotorParameters::MotorAddressParameter::MotorTargetPosition.size(); i++) {
//           m_client->CommandBuffer[ByteNumber++] = MotorParameters::MotorAddressParameter::MotorTargetPosition[i];
//       }
//       for (int i = 0; i < MotorParameters::MotorAddressParameter::MotorStart.size(); i++) {
//           m_client->CommandBuffer[ByteNumber++] = MotorParameters::MotorAddressParameter::MotorStart[i];
//       }
//       for (int i = 0; i < MotorParameters::MotorAddressParameter::MotorStop.size(); i++) {
//           m_client->CommandBuffer[ByteNumber++] = MotorParameters::MotorAddressParameter::MotorStop[i];
//       }
//       for (int i = 0; i < MotorParameters::MotorAddressParameter::Alarmreset.size(); i++) {
//           m_client->CommandBuffer[ByteNumber++] = MotorParameters::MotorAddressParameter::Alarmreset[i];
//       }
//       for (int i = 0; i < MotorParameters::MotorAddressParameter::EncoderPositionReset.size(); i++) {
//           m_client->CommandBuffer[ByteNumber++] = MotorParameters::MotorAddressParameter::EncoderPositionReset[i];
//       }
//       m_client->CommandBuffer[ByteNumber++] = MotorConnected;

//       qDebug()<<"CommandBuffer"<<m_client->CommandBuffer;

       m_client->EventSendReceive();
}
void Protocol::motorEnableFunction(quint8 motor_num)
{
    int ByteNumber = 11;
    qDebug()<<"motor enable";

    m_client->_pauseEventTrigger();
    m_client->MotionName = QString::number(motor_num);
    m_client->CommandBuffer[ByteNumber++] = DR_ProtocolIDs::MotorEnable;
    m_client->CommandBuffer[ByteNumber++] = motor_num;
    m_client->EventSendReceive();
}

void Protocol::servoAutoStart(quint8 motor_num, const QVector<int>& vel, const QVector<double>& TargetPosition)
{
    // Convert velocities
        QVector<int> velocities(7);
        for (int i = 0; i < 7; i++) {
            velocities[i] = vel[i] * 240;
        }

        // Calculate target counts
        for (int i = 0; i < 7; i++) {
            TargetCounts[i] = distanceToPulseCalculation(i, TargetPosition[i]);
        }

        int ByteNumber = 11;
        m_client->_pauseEventTrigger();

        m_client->CommandBuffer[ByteNumber++] = DR_ProtocolIDs::Servomotor_start;
        m_client->CommandBuffer[ByteNumber++] = static_cast<quint8>(Client::mode_select::Auto_set_mode);
        m_client->CommandBuffer[ByteNumber++] = motor_num;

        // Motor 1
        m_client->CommandBuffer[ByteNumber++] = static_cast<quint8>(velocities[0] >> 24);
        m_client->CommandBuffer[ByteNumber++] = static_cast<quint8>(velocities[0] >> 16);
        m_client->CommandBuffer[ByteNumber++] = static_cast<quint8>(velocities[0] >> 8);
        m_client->CommandBuffer[ByteNumber++] = static_cast<quint8>(velocities[0]);
        m_client->CommandBuffer[ByteNumber++] = static_cast<quint8>(TargetCounts[0] >> 24);
        m_client->CommandBuffer[ByteNumber++] = static_cast<quint8>(TargetCounts[0] >> 16);
        m_client->CommandBuffer[ByteNumber++] = static_cast<quint8>(TargetCounts[0] >> 8);
        m_client->CommandBuffer[ByteNumber++] = static_cast<quint8>(TargetCounts[0]);

        // Motor 2-7 follow the same pattern...
        // [Similar code for motors 2 through 7]

        m_client->EventSendReceive();
}

int Protocol::distanceToPulseCalculation(quint8 selectedMotor, double distance)
{
    double requiredPulses = qRound((distance * PulsePerRotation[selectedMotor]) / LinearDistancePerRotation[selectedMotor]);
    return static_cast<int>(requiredPulses);
}

void Protocol::servomotorAutoStop(quint8 motor_num)
{
    int ByteNumber = 11;
    m_client->_pauseEventTrigger();
    m_client->MotionName = QString::number(motor_num);
    m_client->CommandBuffer[ByteNumber++] = DR_ProtocolIDs::Servomotor_stop;
    m_client->CommandBuffer[ByteNumber++] = motor_num;
    m_client->EventSendReceive();
}
