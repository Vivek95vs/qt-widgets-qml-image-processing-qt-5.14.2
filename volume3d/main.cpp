#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QQuickWindow>
#include <QDebug>
#include "DicomHandler.h"
#include "DicomImageProvider.h"

int main(int argc, char *argv[])
{
    // Set core attributes before creating app
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QCoreApplication::setAttribute(Qt::AA_UseHighDpiPixmaps);

    // Force software rendering to avoid OpenGL issues
    QQuickWindow::setSceneGraphBackend(QSGRendererInterface::Software);

    QGuiApplication app(argc, argv);
    app.setApplicationName("DICOM Viewer");
    app.setOrganizationName("Medical");

    qDebug() << "Starting DICOM Viewer...";

    // Create QML engine
    QQmlApplicationEngine engine;

    // Create image provider
    DicomImageProvider* imageProvider = new DicomImageProvider();

    // Create DICOM handler
    DicomHandler* dicomHandler = new DicomHandler(imageProvider, nullptr);

    // Register with QML engine
    engine.addImageProvider("dicom", imageProvider);
    engine.rootContext()->setContextProperty("dicomHandler", dicomHandler);

    // Load main QML file
    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));

    if (engine.rootObjects().isEmpty()) {
        qCritical() << "Failed to load QML!";
        delete dicomHandler;
        delete imageProvider;
        return -1;
    }

    qDebug() << "Application started successfully";

    // Cleanup on exit
    QObject::connect(&app, &QGuiApplication::aboutToQuit, [=]() {
        qDebug() << "Cleaning up...";
        delete dicomHandler;
        delete imageProvider;
    });

    return app.exec();
}
