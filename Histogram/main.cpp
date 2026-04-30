#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include "rawimageload.h"
#include <QDebug>

int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;

    // Register the RawImageLoad class as an image provider
    RawImageLoad imageLoad;
    engine.addImageProvider("imageProvider", &imageLoad);
    engine.rootContext()->setContextProperty("imageMagnifier", &imageLoad); // Expose the object to QML
    qDebug() << "Image provider registered successfully!";

    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));
    if (engine.rootObjects().isEmpty())
        return -1;

    return app.exec();
}
