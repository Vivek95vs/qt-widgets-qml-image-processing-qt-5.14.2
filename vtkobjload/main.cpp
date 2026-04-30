#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQuickWindow>

#include <vtkAutoInit.h>

VTK_MODULE_INIT(vtkRenderingOpenGL2)
VTK_MODULE_INIT(vtkInteractionStyle)
VTK_MODULE_INIT(vtkRenderingVolumeOpenGL2)

#include "volumeitem.h"



int main(int argc, char *argv[])
{
    QQuickWindow::setSceneGraphBackend(QSGRendererInterface::OpenGL);
    QGuiApplication app(argc, argv);

    qmlRegisterType<VolumeItem>("VTK", 1, 0, "VolumeView");

    QQmlApplicationEngine engine;
    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));

    return app.exec();
}
