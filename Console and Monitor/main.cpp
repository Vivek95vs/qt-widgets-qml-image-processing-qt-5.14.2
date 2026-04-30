#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QQmlEngine>
#include <QQmlComponent>

// Register DrawingManager as a singleton
static QObject *drawingManagerSingletonProvider(QQmlEngine *engine, QJSEngine *scriptEngine)
{
    Q_UNUSED(engine)
    Q_UNUSED(scriptEngine)

    // Create and return the singleton instance
    QQmlComponent component(engine, QUrl(QStringLiteral("qrc:/DrawingManager.qml")));
    return component.create();
}

int main(int argc, char *argv[])
{
    qputenv("QT_IM_MODULE", QByteArray("qtvirtualkeyboard"));
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    QGuiApplication app(argc, argv);

    // Set application info
    app.setApplicationName("C-Arm");
    app.setOrganizationName("Panacea Medical Technologies");

    QQmlApplicationEngine engine;

    // Register DrawingManager as a singleton
    qmlRegisterSingletonType<QObject>("DrawingManager", 1, 0, "DrawingManager", drawingManagerSingletonProvider);

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
