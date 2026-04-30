#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QSurfaceFormat>

#include "DicomHandler.h"
#include "DicomImageProvider.h"
#include "VtkVolumeRenderer.h"
#include "VtkQmlIntegration.h"

int main(int argc, char *argv[]) {
    // Set OpenGL requirements
    QSurfaceFormat format;
    format.setRenderableType(QSurfaceFormat::OpenGL);
    format.setProfile(QSurfaceFormat::CoreProfile);
    format.setVersion(3, 3);
    format.setSamples(4); // MSAA
    QSurfaceFormat::setDefaultFormat(format);

    qputenv("QT_OPENGL", "angle");
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QCoreApplication::setAttribute(Qt::AA_UseHighDpiPixmaps);

    QGuiApplication app(argc, argv);

    // Register QML types
    qmlRegisterType<VtkQmlItem>("MedicalImaging", 1, 0, "VtkRenderer");

    QQmlApplicationEngine engine;

    // Create providers
    DicomImageProvider* imageProvider = new DicomImageProvider;
    DicomHandler* dicomHandler = new DicomHandler(imageProvider);
    VtkVolumeRenderer* vtkRenderer = new VtkVolumeRenderer;

    // Connect signals
    QObject::connect(dicomHandler, &DicomHandler::volumeLoaded, [=]() {
        auto volumeData = dicomHandler->getVolumeData();
        auto dims = dicomHandler->volumeDimensions();
        vtkRenderer->loadVolumeData(volumeData, dims.x(), dims.y(), dims.z());
    });

    // Expose to QML
    engine.rootContext()->setContextProperty("dicomHandler", dicomHandler);
    engine.rootContext()->setContextProperty("vtkRenderer", vtkRenderer);
    engine.addImageProvider("dicom", imageProvider);

    // Load main QML
    const QUrl url(QStringLiteral("qrc:/main.qml"));
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
                     &app, [url](QObject* obj, const QUrl& objUrl) {
        if (!obj && url == objUrl)
            QCoreApplication::exit(-1);
    }, Qt::QueuedConnection);

    engine.load(url);

    return app.exec();
}
