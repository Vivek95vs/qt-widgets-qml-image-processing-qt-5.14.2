#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QQuickVTKRenderWindow.h>
#include <QQuickVTKRenderItem.h>
#include "VolumeRenderer.h"
#include "DicomHandler.h"
#include "DicomImageProvider.h"


#include <QGuiApplication>
#include <QQmlApplicationEngine>

int main(int argc, char *argv[])
{

    QGuiApplication app(argc, argv);

    qmlRegisterType<QQuickVTKRenderWindow>("VTK", 9, 2, "VTKRenderWindow");
    qmlRegisterType<QQuickVTKRenderItem>("VTK", 9, 2, "VTKRenderItem");

    // Register DicomController to QML under "com.example.DicomController"
    qmlRegisterType<VolumeRenderer>("com.example.DicomController", 1, 0, "DicomController");

    VolumeRenderer controller;
    QQmlApplicationEngine engine;

    // Expose the DicomController to QML as "dicomController"
    engine.rootContext()->setContextProperty("VolumeRenderer", &controller);

    // Create providers
    DicomImageProvider* imageProvider = new DicomImageProvider();
    DicomHandler* dicomHandler = new DicomHandler();

    // Connect signals between handler and provider
    QObject::connect(dicomHandler, &DicomHandler::crosshairPositionChanged,
                     [imageProvider, dicomHandler](const QVector3D& pos) {
                         imageProvider->setCrosshairPosition(pos, dicomHandler->getVolumeDimensions());
                     });

    QObject::connect(dicomHandler, &DicomHandler::sliceUpdated,
                     [imageProvider, dicomHandler](int viewType, int sliceIndex) {
                         QImage slice = dicomHandler->getSlice(sliceIndex, viewType);
                         imageProvider->setSliceImage(viewType, slice);
                     });

    // Register with QML
    engine.rootContext()->setContextProperty("dicomHandler", dicomHandler);
    engine.addImageProvider("dicom", imageProvider);

    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));

    return app.exec();
}
