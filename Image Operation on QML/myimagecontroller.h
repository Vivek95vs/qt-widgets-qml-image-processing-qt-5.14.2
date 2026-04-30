#ifndef IMAGECONTROLLER_H
#define IMAGECONTROLLER_H

#include <QObject>
#include <QImage>
#include "myimageprovider.h"
#include <opencv2/opencv.hpp>
#include "myserver.h"
#include "tcppollthread.h"


class ImageController : public QObject {
    Q_OBJECT
public:
    explicit ImageController(MyImageProvider *provider, QObject *parent = nullptr);
     //QString hello() const;
    MyServer Server;
    TcpPollThread m_pollThread;


public slots:
    void loadRawImage();
    void scatterCorrection();
    void enhanceEdges();
    void medianFilter();
    void flipImage_vertically();
    void flipImage_horizontally();

    cv::Mat QImageToCvMat(const QImage &inImage);
signals:
    void imageUpdated();

private:
    MyImageProvider *imageProvider;
    QImage img1;
};

#endif // IMAGECONTROLLER_H
