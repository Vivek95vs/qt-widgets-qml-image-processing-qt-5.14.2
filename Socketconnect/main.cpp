#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include "SocketClient.h"
#include "ProtocolHandler.h"

int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QGuiApplication app(argc, argv);

    // Register both types with the QML system
    qmlRegisterType<SocketClient>("DRController", 1, 0, "SocketClient");
    qmlRegisterType<ProtocolHandler>("DRController", 1, 0, "ProtocolHandler");

    QQmlApplicationEngine engine;
    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));

    if (engine.rootObjects().isEmpty())
        return -1;

    return app.exec();
}
