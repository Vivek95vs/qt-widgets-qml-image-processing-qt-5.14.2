#pragma once
#include <QQuickItem>
#include <QQuickWindow>
#include <QOpenGLFunctions>

#include <vtkGenericOpenGLRenderWindow.h>
#include <vtkRenderer.h>
#include <QVTKOpenGLWidget.h>  // Changed from QVTKOpenGLNativeWidget

class VtkQmlItem : public QQuickItem {
    Q_OBJECT
    Q_PROPERTY(bool volumeVisible READ volumeVisible WRITE setVolumeVisible NOTIFY volumeVisibleChanged)

public:
    VtkQmlItem(QQuickItem* parent = nullptr);
    ~VtkQmlItem();

    bool volumeVisible() const { return m_volumeVisible; }
    void setVolumeVisible(bool visible);

    void setRenderer(vtkRenderer* renderer);
    void setRenderWindow(vtkGenericOpenGLRenderWindow* renderWindow);

public slots:
    void sync();
    void cleanup();

signals:
    void volumeVisibleChanged();

protected:
    QSGNode* updatePaintNode(QSGNode* oldNode, UpdatePaintNodeData* data) override;

private slots:
    void handleWindowChanged(QQuickWindow* window);

private:
    void init();
    void updateSize();

    vtkSmartPointer<vtkGenericOpenGLRenderWindow> m_renderWindow;
    vtkSmartPointer<vtkRenderer> m_renderer;
    QVTKOpenGLWidget* m_vtkWidget;  // Changed from QVTKOpenGLNativeWidget
    bool m_volumeVisible;
    bool m_initialized;
    QQuickWindow* m_window;
};
