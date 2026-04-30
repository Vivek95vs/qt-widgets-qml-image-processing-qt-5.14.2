#include <QtWidgets/QApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QQuickStyle>
#include "contourbackend.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    // Set application info
    app.setApplicationName("Contour Drawing App");
    app.setOrganizationName("YourCompany");
    app.setOrganizationDomain("yourcompany.com");

    // Set style
    QQuickStyle::setStyle("Material");

    // Register C++ type with QML
    qmlRegisterType<ContourBackend>("Contour.Backend", 1, 0, "ContourBackend");

    // Create and configure engine
    QQmlApplicationEngine engine;

    // Load QML file
    const QUrl url(QStringLiteral("qrc:/main.qml"));

    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
                     &app, [url](QObject *obj, const QUrl &objUrl) {
        if (!obj && url == objUrl)
            QCoreApplication::exit(-1);
    }, Qt::QueuedConnection);

    engine.load(url);

    return app.exec();
}
