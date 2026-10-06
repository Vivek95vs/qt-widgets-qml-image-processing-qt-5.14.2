#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include "bonesuppressionprocessor.h"

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    qmlRegisterType<BoneSuppressionProcessor>("com.bonesuppression", 1, 0, "BoneSuppressionProcessor");

    QQmlApplicationEngine engine;
    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));

    return app.exec();
}
