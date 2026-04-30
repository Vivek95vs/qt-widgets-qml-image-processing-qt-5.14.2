#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QQuickVTKRenderWindow.h>
#include <QQuickVTKRenderItem.h>
#include "DicomController.h"
#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include "rawimageprovider.h"
int main(int argc, char *argv[])
{

    QGuiApplication app(argc, argv);

    qmlRegisterType<QQuickVTKRenderWindow>("VTK", 9, 2, "VTKRenderWindow");
    qmlRegisterType<QQuickVTKRenderItem>("VTK", 9, 2, "VTKRenderItem");

    // Register DicomController to QML under "com.example.DicomController"
    qmlRegisterType<DicomController>("com.example.DicomController", 1, 0, "DicomController");

    DicomController controller;
    QQmlApplicationEngine engine;

    // Expose the DicomController to QML as "dicomController"
    engine.rootContext()->setContextProperty("dicomController", &controller);
    engine.addImageProvider(QLatin1String("raw"), new RawImageProvider);

    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));

    return app.exec();
}
