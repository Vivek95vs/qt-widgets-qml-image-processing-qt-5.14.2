#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include "imagemagnifier.h"
#include "QDebug"

int main(int argc, char *argv[]) {
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;

    // Register the ImageMagnifier class as an image provider
    ImageMagnifier imageMagnifier;
    engine.addImageProvider("imageProvider", &imageMagnifier);
    engine.rootContext()->setContextProperty("imageMagnifier", &imageMagnifier);
    qDebug() << "Image provider registered successfully!";

    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));
    if (engine.rootObjects().isEmpty())
        return -1;

    return app.exec();
}
