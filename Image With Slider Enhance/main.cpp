#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include "DicomHandler.h"
#include "DicomImageProvider.h"
#include <QIcon>
#include <QDir>

int main(int argc, char *argv[]) {
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;

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

    // Load QML
    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));

    return app.exec();
}
