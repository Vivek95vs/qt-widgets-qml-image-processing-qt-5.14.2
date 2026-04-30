#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include "mainwindow.h"

int main(int argc, char *argv[])
{
    qputenv("QT_OPENGL", "angle");  // Use ANGLE (OpenGLES over DirectX) gpu without flickering
    //    QCoreApplication::setAttribute(Qt::AA_UseDesktopOpenGL);    //gpu with fickering
    //    QCoreApplication::setAttribute(Qt::AA_UseOpenGLES);   // If OpenGL ES is supported gpu without flickering
    //    QCoreApplication::setAttribute(Qt::AA_UseSoftwareOpenGL);
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
     mainwindow();
    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;
    const QUrl url(QStringLiteral("qrc:/main.qml"));
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
                     &app, [url](QObject *obj, const QUrl &objUrl) {
        if (!obj && url == objUrl)
            QCoreApplication::exit(-1);
    }, Qt::QueuedConnection);
    engine.load(url);

    return app.exec();
}
