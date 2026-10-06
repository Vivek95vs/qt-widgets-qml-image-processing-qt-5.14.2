#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>

#include "dicomloader.h"
#include "dicomimageprovider.h"

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;

    // =========================================================
    // DICOM LOADER
    // =========================================================

    DicomLoader dicomLoader;

    // =========================================================
    // EXPOSE DICOM LOADER TO QML
    // =========================================================

    engine.rootContext()->setContextProperty(
                "dicomLoader",
                &dicomLoader);

    // =========================================================
    // DICOM IMAGE PROVIDER
    // =========================================================

    engine.addImageProvider(
                "dicom",
                new DicomImageProvider(
                    &dicomLoader));

    // =========================================================
    // LOAD MAIN QML
    // =========================================================

    engine.load(
                QUrl(
                    QStringLiteral(
                        "qrc:/main.qml")));

    // =========================================================
    // CHECK QML LOAD
    // =========================================================

    if (engine.rootObjects().isEmpty())
    {
        return -1;
    }

    return app.exec();
}
