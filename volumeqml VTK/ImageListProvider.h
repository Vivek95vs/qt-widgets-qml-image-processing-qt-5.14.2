// File: ImageListProvider.h
#pragma once
#include <QObject>
#include <QStringList>
#include <QDir>

class ImageListProvider : public QObject {
    Q_OBJECT
    Q_PROPERTY(QStringList imagePaths READ imagePaths NOTIFY imagePathsChanged)

public:
    explicit ImageListProvider(QObject* parent = nullptr) : QObject(parent) {
        loadImages("D:/Images");  // change this to your folder
    }

    QStringList imagePaths() const { return m_imagePaths; }

signals:
    void imagePathsChanged();

public slots:
    void loadImages(const QString& folderPath) {
        QDir dir(folderPath);
        QStringList filters;
        filters << "*.png" << "*.jpg" << "*.jpeg" << "*.bmp";
        m_imagePaths = dir.entryList(filters, QDir::Files);
        for (int i = 0; i < m_imagePaths.size(); ++i)
            m_imagePaths[i] = "file:///" + folderPath + "/" + m_imagePaths[i];
        emit imagePathsChanged();
    }

private:
    QStringList m_imagePaths;
};
