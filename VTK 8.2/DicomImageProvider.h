#pragma once
#include <QQuickImageProvider>
#include <QImage>

class DicomImageProvider : public QQuickImageProvider {
public:
    DicomImageProvider() : QQuickImageProvider(QQuickImageProvider::Image) {}

    void setImage(const QImage& image) {
        m_image = image;
    }
    void clearImage() {
        m_image = QImage(); // Clear to empty
    }

    QImage requestImage(const QString&, QSize* size, const QSize&) override {
        if (size) *size = m_image.size();
        return m_image;
    }

private:
    QImage m_image;
};
