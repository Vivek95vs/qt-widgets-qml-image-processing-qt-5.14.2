#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include "DicomHandler.h"
#include <QCoreApplication>

int main(int argc, char *argv[]) {
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QCoreApplication::setAttribute(Qt::AA_UseHighDpiPixmaps);

    qputenv("QT_OPENGL", "angle");
    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;

    // Create handler
    DicomHandler* dicomHandler = new DicomHandler();

    // Register image providers
    engine.addImageProvider("axial", dicomHandler->axialImageProvider());
    engine.addImageProvider("coronal", dicomHandler->coronalImageProvider());
    engine.addImageProvider("sagittal", dicomHandler->sagittalImageProvider());

    engine.rootContext()->setContextProperty("dicomHandler", dicomHandler);

    const QUrl url(QStringLiteral("qrc:/main.qml"));
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
                     &app, [url](QObject* obj, const QUrl& objUrl) {
        if (!obj && url == objUrl)
            QCoreApplication::exit(-1);
    }, Qt::QueuedConnection);

    engine.load(url);

    return app.exec();
}
