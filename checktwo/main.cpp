#include "QCoreApplication"
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QQuickWindow>
#include <QImage>
#include <QPainter>
#include <QObject>
#include <QVector>
#include <QPointF>
#include <QMutex>
#include <QStandardPaths>
#include <QDir>
#include <QDebug>
#include <QFileDialog>
#include <QQuickStyle>
#include <QApplication>

// Shared drawing data class
class DrawingData : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString imageSource READ imageSource WRITE setImageSource NOTIFY imageSourceChanged)
    Q_PROPERTY(QSize originalImageSize READ originalImageSize NOTIFY originalImageSizeChanged)
    Q_PROPERTY(bool imageLoaded READ imageLoaded NOTIFY imageLoadedChanged)

public:
    explicit DrawingData(QObject *parent = nullptr) : QObject(parent), m_imageLoaded(false) {}

    QString imageSource() const { return m_imageSource; }
    QSize originalImageSize() const { return m_originalImage.size(); }
    bool imageLoaded() const { return m_imageLoaded; }

    void setImageSource(const QString &source) {
        if (m_imageSource != source) {
            m_imageSource = source;

            // Check if file exists
            QString filePath = source;
            if (filePath.startsWith("file:///")) {
                filePath = filePath.mid(8); // Remove "file:///"
            }

            QFile file(filePath);
            if (!file.exists()) {
                qDebug() << "File does not exist:" << filePath;
                m_imageLoaded = false;
                emit imageLoadedChanged(false);
                return;
            }

            // Load image from file path
            if (m_originalImage.load(filePath)) {
                m_imageLoaded = true;
                qDebug() << "Image loaded successfully:" << filePath;
                qDebug() << "Image size:" << m_originalImage.size();

                // Clear any existing drawings
                clearDrawings();

                emit imageSourceChanged(source);
                emit originalImageSizeChanged(m_originalImage.size());
                emit imageLoadedChanged(true);
            } else {
                m_imageLoaded = false;
                qDebug() << "Failed to load image:" << filePath;
                emit imageLoadedChanged(false);
            }
        }
    }

    // Add a drawing point (in image coordinates)
    Q_INVOKABLE void addPoint(qreal x, qreal y, const QString &color) {
        QMutexLocker locker(&m_mutex);
        m_points.append({QPointF(x, y), QColor(color)});
        emit drawingChanged();
    }

    // Clear all drawings
    Q_INVOKABLE void clearDrawings() {
        QMutexLocker locker(&m_mutex);
        m_points.clear();
        emit drawingChanged();
    }

    // Get all drawing points
    QVector<QPair<QPointF, QColor>> getPoints() const {
        QMutexLocker locker(&m_mutex);
        return m_points;
    }

    // Get the original image
    QImage getOriginalImage() const {
        return m_originalImage;
    }

signals:
    void imageSourceChanged(const QString &source);
    void originalImageSizeChanged(const QSize &size);
    void imageLoadedChanged(bool loaded);
    void drawingChanged();

private:
    QString m_imageSource;
    QImage m_originalImage;
    QVector<QPair<QPointF, QColor>> m_points;
    mutable QMutex m_mutex;
    bool m_imageLoaded;
};

// Window manager to handle multiple windows
class WindowManager : public QObject
{
    Q_OBJECT

public:
    explicit WindowManager(QObject *parent = nullptr) : QObject(parent) {}

    Q_INVOKABLE void createSecondaryWindow() {
        if (!m_secondaryEngine) {
            m_secondaryEngine = new QQmlApplicationEngine(this);

            // Get the drawing data from main engine context
            auto context = m_secondaryEngine->rootContext();
            context->setContextProperty("drawingData", m_drawingData);

            // Load secondary QML
            m_secondaryEngine->load(QUrl("qrc:/SecondaryWindow.qml"));

            if (!m_secondaryEngine->rootObjects().isEmpty()) {
                QQuickWindow *secondaryWindow = qobject_cast<QQuickWindow*>(m_secondaryEngine->rootObjects().first());
                if (secondaryWindow) {
                    secondaryWindow->setTitle("Secondary Window - 1280x1024");
                    secondaryWindow->resize(1280, 1024);
                    secondaryWindow->show();

                    // Position secondary window next to primary
                    secondaryWindow->setPosition(2000, 100);
                }
            }
        }
    }

    void setDrawingData(DrawingData *data) {
        m_drawingData = data;
    }

private:
    QQmlApplicationEngine *m_secondaryEngine = nullptr;
    DrawingData *m_drawingData = nullptr;
};

int main(int argc, char *argv[])
{
    // Set OpenGL backend for better performance
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QCoreApplication::setAttribute(Qt::AA_UseDesktopOpenGL);

    QApplication app(argc, argv);
    app.setApplicationName("Multi-Window Drawing");
    app.setOrganizationName("MyCompany");

    // Set modern style
    QQuickStyle::setStyle("Material");

    // Create shared drawing data
    DrawingData *drawingData = new DrawingData();

    // Create window manager
    WindowManager *windowManager = new WindowManager();
    windowManager->setDrawingData(drawingData);

    // Set up main engine
    QQmlApplicationEngine engine;
    QQmlContext *context = engine.rootContext();
    context->setContextProperty("drawingData", drawingData);
    context->setContextProperty("windowManager", windowManager);

    // Load main QML - IMPORTANT: This should be "qrc:/main.qml"
    engine.load(QUrl("qrc:/main.qml"));

    if (engine.rootObjects().isEmpty())
        return -1;

    // Get main window and set properties
    QQuickWindow *mainWindow = qobject_cast<QQuickWindow*>(engine.rootObjects().first());
    if (mainWindow) {
        mainWindow->setTitle("Primary Window - 1920x1200");
        mainWindow->resize(1920, 1200);
        mainWindow->show();

        // Position window
        mainWindow->setPosition(100, 100);
    }

    return app.exec();
}

#include "main.moc"
