#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include "BackendManager.h"

int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QGuiApplication app(argc, argv);

    // Register backend types
    qmlRegisterType<BackendManager>("DigitalRadiography", 1, 0, "BackendManager");

    QQmlApplicationEngine engine;
    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));
    if (engine.rootObjects().isEmpty())
        return -1;

    return app.exec();
}
