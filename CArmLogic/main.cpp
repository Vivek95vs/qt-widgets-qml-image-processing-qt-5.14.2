#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include "storageinfo.h"

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);
    QQmlApplicationEngine engine;

    StorageInfo storage;
    engine.rootContext()->setContextProperty("StorageInfo", &storage);

    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));

    return app.exec();
}
