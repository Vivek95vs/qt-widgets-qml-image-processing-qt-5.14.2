#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QQmlEngine>
#include <QQuickStyle>

int main(int argc, char *argv[])
{
    qputenv("QT_IM_MODULE", QByteArray("qtvirtualkeyboard"));
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    QGuiApplication app(argc, argv);

    // Set application info
    app.setApplicationName("C-Arm");
    app.setOrganizationName("Panacea Medical Technologies");
    app.setOrganizationDomain("panaceamedical.com");

    QQmlApplicationEngine engine;

    // Register singleton type
    qmlRegisterSingletonType(QUrl("qrc:/DrawingManager.qml"), "DrawingManager", 1, 0, "DrawingManager");

    QString appVersion = "1.0.0_A1";
    engine.rootContext()->setContextProperty("appVersion", appVersion);

    QString hospitalname = "Panacea Medical Technologies";
    engine.rootContext()->setContextProperty("hospitalname", hospitalname);

    engine.rootContext()->setContextProperty("screenCount", QGuiApplication::screens().count());

    const QUrl url(QStringLiteral("qrc:/main.qml"));
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
                     &app, [url](QObject *obj, const QUrl &objUrl) {
        if (!obj && url == objUrl)
            QCoreApplication::exit(-1);
    }, Qt::QueuedConnection);

    engine.load(url);

    return app.exec();
}
