#ifndef VOLUMEITEM_H
#define VOLUMEITEM_H

#include <QQuickFramebufferObject>
#include <vtkSmartPointer.h>

class VolumeItem : public QQuickFramebufferObject
{
    Q_OBJECT
public:
    QQuickFramebufferObject::Renderer *createRenderer() const override;

    Q_INVOKABLE void loadVTI(const QString &path);
    QString vtiPath;
};

#endif
