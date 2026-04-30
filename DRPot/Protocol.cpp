#include "Protocol.h"

QThread* Protocol::Sequence_thread = nullptr;
QThread* Protocol::Dr_BoardFeedBack_thread = nullptr;

QString Protocol::WorkdflowfilePath = "D:/Digital_Radiography/Log Files";
QString Protocol::WorkdflowfileName = "Workflow.txt";
QString Protocol::Log_Workflowpath = "D:/Digital_Radiography/Log Files/Wrokflow.txt";
QString Protocol::ReadDataFromIA_Filepath = "D:/Digital_Radiography/Image_Aquasition/SendDataToBackendFromIA/ImageAquasitionData.txt";
QString Protocol::ReadDataFromDatabase_Filepath = "D:/Digital_Radiography/Data_Base/SendDataToBackend/DatabaseValue.txt";

// Other static member initializations...

Protocol::Protocol(QObject *parent) : QObject(parent)
{
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
}
