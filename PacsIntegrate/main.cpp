#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QDebug>
#include <QFile>
#include "dicommanager.h"

int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QGuiApplication app(argc, argv);

    qmlRegisterType<DicomManager>("DicomApp", 1, 0, "DicomManager");

    QQmlApplicationEngine engine;

    // Create and expose DicomManager
    DicomManager dicomManager;
    engine.rootContext()->setContextProperty("dicomManager", &dicomManager);

    // Load configuration
//    QString configPath = "D:\\Qt 2017 program\\PacsIntegrate\\config.ini";

    // Load configuration
    QString configPath = ":/config.ini";

    // Check if config exists in resources
    if (!QFile::exists(configPath)) {
        qDebug() << "Config file not found in resources, using local config.ini";
        configPath = "D:\\Qt 2017 program\\PacsIntegrate\\config.ini";
    }
    dicomManager.loadConfiguration(configPath);

    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));
    if (engine.rootObjects().isEmpty())
        return -1;

    return app.exec();
}
