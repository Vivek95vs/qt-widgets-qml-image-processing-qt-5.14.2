#include "myimagecontroller.h"
#include "myimageprovider.h"
#include "main.h"
#include "servercontroller.h"

int main(int argc, char *argv[]) {
    QGuiApplication app(argc, argv);
    QQmlApplicationEngine engine;


    ServerController serverController;
    engine.rootContext()->setContextProperty("serverController", &serverController);
    MyImageProvider *imageProvider = new MyImageProvider();
    engine.addImageProvider(QLatin1String("multiImageProvider"), imageProvider);

    ImageController imageController(imageProvider);
    engine.rootContext()->setContextProperty("imageController", &imageController);    
    imageProvider->addRawImage("img1", "D:/proj_mreged_crop.raw", 3072, 3072);
//    imageProvider->addRawImage("img2", "D:/proj_mreged_crop.raw", 3072, 3072);

    imageController.loadRawImage();
    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));
    if (engine.rootObjects().isEmpty()) return -1;

    return app.exec();
}

