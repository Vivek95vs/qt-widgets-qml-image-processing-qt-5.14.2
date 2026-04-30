#include "VtkQmlIntegration.h"
#include <QOpenGLContext>
#include <QQuickWindow>
#include <QGuiApplication>
#include <QDebug>

VtkQmlItem::VtkQmlItem(QQuickItem* parent)
    : QQuickItem(parent), m_volumeVisible(true),
      m_initialized(false), m_window(nullptr) {

    setFlag(ItemHasContents, true);
    setFlag(ItemHasContents, true);
    connect(this, &QQuickItem::windowChanged, this, &VtkQmlItem::handleWindowChanged);
}

VtkQmlItem::~VtkQmlItem() {
    if (m_vtkWidget) {
        m_vtkWidget->deleteLater();
    }
}

void VtkQmlItem::setVolumeVisible(bool visible) {
    if (m_volumeVisible == visible) return;
    m_volumeVisible = visible;

    if (m_renderer && m_renderer->GetActors()->GetNumberOfItems() > 0) {
        // Set visibility of all actors
        m_renderer->SetDraw(visible);
        update();
    }

    emit volumeVisibleChanged();
}

void VtkQmlItem::setRenderer(vtkRenderer* renderer) {
    m_renderer = renderer;
    if (m_vtkWidget && m_renderer) {
        m_vtkWidget->renderWindow()->AddRenderer(m_renderer);
    }
}

void VtkQmlItem::setRenderWindow(vtkGenericOpenGLRenderWindow* renderWindow) {
    m_renderWindow = renderWindow;
    if (m_vtkWidget && m_renderWindow) {
        m_vtkWidget->setRenderWindow(m_renderWindow);
    }
}

void VtkQmlItem::handleWindowChanged(QQuickWindow* window) {
    if (window) {
        m_window = window;
        connect(window, &QQuickWindow::beforeSynchronizing, this, &VtkQmlItem::sync, Qt::DirectConnection);
        connect(window, &QQuickWindow::sceneGraphInvalidated, this, &VtkQmlItem::cleanup, Qt::DirectConnection);

        if (!m_initialized) {
            QTimer::singleShot(100, this, &VtkQmlItem::init);
        }
    }
}

void VtkQmlItem::init() {
    if (m_initialized) return;

    m_vtkWidget = new QVTKOpenGLWidget();

    if (m_renderWindow) {
        m_vtkWidget->setRenderWindow(m_renderWindow);
    } else if (m_renderer) {
        auto renderWindow = vtkSmartPointer<vtkGenericOpenGLRenderWindow>::New();
        renderWindow->AddRenderer(m_renderer);
        m_vtkWidget->setRenderWindow(renderWindow);
    }

    m_vtkWidget->setVisible(true);
    m_vtkWidget->setParent(nullptr); // No parent for embedding
    m_vtkWidget->show();

    // Create a container window for VTK widget
    QWindow* container = QWindow::fromWinId(m_vtkWidget->winId());
    if (container) {
        container->setParent(m_window);
        container->setGeometry(QRect(mapToScene(QPointF(0, 0)).toPoint(),
                                   QSize(width(), height())));
    }

    m_initialized = true;
}

void VtkQmlItem::sync() {
    if (!m_vtkWidget || !m_initialized || !m_window) return;

    updateSize();
}

void VtkQmlItem::cleanup() {
    if (m_vtkWidget) {
        m_vtkWidget->deleteLater();
        m_vtkWidget = nullptr;
    }
    m_initialized = false;
}

void VtkQmlItem::updateSize() {
    if (!m_vtkWidget) return;

    // Map position from QML item to window coordinates
    QPointF scenePos = mapToScene(QPointF(0, 0));
    QPoint windowPos = m_window->mapFromScene(scenePos).toPoint();

    // Update VTK widget geometry
    m_vtkWidget->setGeometry(QRect(windowPos, QSize(width(), height())));

    // Update render window size
    if (m_renderWindow) {
        m_renderWindow->SetSize(width(), height());
    }
}

QSGNode* VtkQmlItem::updatePaintNode(QSGNode* oldNode, UpdatePaintNodeData* data) {
    // We're using a separate window for VTK, so no QSGNode needed
    Q_UNUSED(data);

    if (!m_vtkWidget || !m_initialized) {
        return oldNode;
    }

    // Trigger a render
    if (m_renderWindow) {
        m_renderWindow->Render();
    }

    // Return empty node
    if (!oldNode) {
        return new QSGNode();
    }

    return oldNode;
}
