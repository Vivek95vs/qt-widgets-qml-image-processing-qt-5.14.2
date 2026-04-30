// main.cpp - Main Application
#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include "LicenseValidator.h"

int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QGuiApplication app(argc, argv);

    QCoreApplication::setOrganizationName("MyCompany");
    QCoreApplication::setApplicationName("MyApp");

    LicenseValidator validator;

    QQmlApplicationEngine engine;
    engine.rootContext()->setContextProperty("licenseValidator", &validator);
    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));

    return app.exec();
}
