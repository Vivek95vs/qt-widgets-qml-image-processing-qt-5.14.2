#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
//#include "DicomImageProvider.h"
#include "DicomHandler.h"
#include <QCoreApplication>
//#include <ImageListProvider.h>

int main(int argc, char *argv[]) {

    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QCoreApplication::setAttribute(Qt::AA_UseHighDpiPixmaps);

//    QCoreApplication::setAttribute(Qt::AA_UseDesktopOpenGL);    //gpu with fickering
//    QCoreApplication::setAttribute(Qt::AA_UseOpenGLES);   // If OpenGL ES is supported gpu without flickering
//    QCoreApplication::setAttribute(Qt::AA_UseSoftwareOpenGL);      //cpu without flickering. this line helps to solve the flickering problem when clicking  windows button and navigation throught differnt UI(.qml)

    qputenv("QT_OPENGL", "angle");  // Use ANGLE (OpenGLES over DirectX) gpu without flickering
    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;

    DicomImageProvider* imageProvider = new DicomImageProvider;
    DicomHandler* dicomHandler = new DicomHandler(imageProvider);
//    ImageListProvider imagelistprovider;

//    engine.rootContext()->setContextProperty("imagelistprovider", &imagelistprovider);
    engine.rootContext()->setContextProperty("dicomHandler", dicomHandler);
    engine.addImageProvider("dicom", imageProvider);

    const QUrl url(QStringLiteral("qrc:/main.qml"));
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
                     &app, [url](QObject* obj, const QUrl& objUrl) {
        if (!obj && url == objUrl)
            QCoreApplication::exit(-1);
    }, Qt::QueuedConnection);
    engine.load(url);

    return app.exec();
}
