#ifndef QMLRENDERER_H
#define QMLRENDERER_H

#include <QObject>
#include <QUrl>

class VTKRenderer;  // Forward declaration

class QMLRenderer : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QUrl imageSource READ imageSource NOTIFY imageSourceChanged)
    Q_PROPERTY(bool rendering READ isRendering NOTIFY renderingChanged)
    Q_PROPERTY(QString statusText READ statusText NOTIFY statusTextChanged)

public:
    explicit QMLRenderer(QObject *parent = nullptr);
    ~QMLRenderer();

    QUrl imageSource() const { return m_imageSource; }
    bool isRendering() const { return m_rendering; }
    QString statusText() const { return m_statusText; }

    Q_INVOKABLE void loadDICOMFolder(const QString &folderPath);
    Q_INVOKABLE void loadSampleData();

signals:
    void imageSourceChanged(const QUrl &source);
    void renderingChanged(bool rendering);
    void statusTextChanged(const QString &status);

private slots:
    void handleRenderingComplete();
    void handleRenderingError(const QString &error);
    void handleRenderImagePathChanged(const QString &path);

private:
    void setRendering(bool rendering);
    void setStatusText(const QString &status);
    void setImageSource(const QUrl &source);

    VTKRenderer *m_vtkRenderer;
    QUrl m_imageSource;
    bool m_rendering;
    QString m_statusText;
};

#endif // QMLRENDERER_H
