// main.cpp
#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QTimer>
#include "collimationmanager.h"

int main(int argc, char *argv[])
{
    QCoreApplication::setOrganizationName("MedicalImaging");
    QCoreApplication::setApplicationName("CollimationControl");

    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;

    CollimationManager collimationManager;
    engine.rootContext()->setContextProperty("collimationManager", &collimationManager);

    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));

    if (engine.rootObjects().isEmpty())
        return -1;

    // Auto-load image
    QTimer::singleShot(500, [&collimationManager]() {
        collimationManager.loadRawImage("D:/DR.raw", 3072, 3072);
    });

    return app.exec();
}
