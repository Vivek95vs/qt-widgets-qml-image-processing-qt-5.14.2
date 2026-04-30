#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QSurfaceFormat>
#include <QQuickStyle>
#include "DicomVolumeRendereDicom.h"

int main(int argc, char *argv[])
{
    // Set OpenGL format for better compatibility
    QSurfaceFormat format;
    format.setRenderableType(QSurfaceFormat::OpenGL);
    format.setProfile(QSurfaceFormat::CoreProfile);
    format.setVersion(3, 3);
    format.setSamples(4);  // 4x MSAA
    QSurfaceFormat::setDefaultFormat(format);

    // Enable high DPI scaling
    QGuiApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    QGuiApplication app(argc, argv);
    app.setApplicationName("DICOM 3D Viewer");
    app.setOrganizationName("Medical Imaging");

    // Register QML type
    qmlRegisterType<DicomVolumeItem>("DicomViewer", 1, 0, "DicomVolumeItem");

    QQmlApplicationEngine engine;

    // Load QML
    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));

    if (engine.rootObjects().isEmpty()) {
        return -1;
    }

    return app.exec();
}
