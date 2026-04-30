#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QQuickVTKRenderWindow.h>
#include <QQuickVTKRenderItem.h>
#include "DicomController.h"

int main(int argc, char *argv[])
{
    QQuickVTKRenderWindow::setupGraphicsBackend();
    QGuiApplication app(argc, argv);

    // This registers the types directly into the engine's memory
    qmlRegisterType<QQuickVTKRenderWindow>("VTK", 9, 2, "VTKRenderWindow");
    qmlRegisterType<QQuickVTKRenderItem>("VTK", 9, 2, "VTKRenderItem");

    DicomController controller;
    QQmlApplicationEngine engine;

    // REMOVE OR COMMENT OUT THIS LINE:
    // engine.addImportPath("D:/vtk9.2/VTK-9.2.6/build/lib/qml/Release");

    engine.rootContext()->setContextProperty("dicomController", &controller);
    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));

    return app.exec();
}
