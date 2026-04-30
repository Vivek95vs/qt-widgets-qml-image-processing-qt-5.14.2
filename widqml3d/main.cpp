#include <QGuiApplication>
#include <QSurfaceFormat>
#include <QQmlApplicationEngine>
#include <QQmlContext>

#include "vtk_render_to_image.h"

// -------- VTK AUTO INIT (MANDATORY) --------
#include <vtkAutoInit.h>
VTK_MODULE_INIT(vtkRenderingOpenGL2)
VTK_MODULE_INIT(vtkInteractionStyle)
VTK_MODULE_INIT(vtkRenderingFreeType)

int main(int argc, char *argv[])
{
    // -------- FORCE DESKTOP OPENGL --------
    QSurfaceFormat fmt;
    fmt.setRenderableType(QSurfaceFormat::OpenGL);
    fmt.setVersion(3, 2);
    fmt.setProfile(QSurfaceFormat::CoreProfile);
    QSurfaceFormat::setDefaultFormat(fmt);

    QGuiApplication app(argc, argv);

    VtkRenderToImage vtkRenderer;

    QQmlApplicationEngine engine;
    engine.rootContext()->setContextProperty("vtkRenderer", &vtkRenderer);
    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));

    return app.exec();
}
