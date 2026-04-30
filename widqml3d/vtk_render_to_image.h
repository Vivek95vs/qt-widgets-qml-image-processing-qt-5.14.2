#ifndef VTK_RENDER_TO_IMAGE_H
#define VTK_RENDER_TO_IMAGE_H

#include <QObject>

class VtkRenderToImage : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString imagePath READ imagePath NOTIFY imagePathChanged)

public:
    explicit VtkRenderToImage(QObject *parent = nullptr);

    Q_INVOKABLE void renderAndSave();

    QString imagePath() const;

signals:
    void imagePathChanged();

private:
    QString m_imagePath;
};

#endif
