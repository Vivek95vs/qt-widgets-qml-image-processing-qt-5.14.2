#ifndef DICOMCONTROLLER_H
#define DICOMCONTROLLER_H

#include <QObject>
#include <QString>
#include <vtkSmartPointer.h>
#include <vtkRenderer.h>
#include <vtkDICOMReader.h>
#include <vtkImageData.h>
#include <vtkRenderWindow.h>

class DicomController : public QObject
{
    Q_OBJECT
    Q_PROPERTY(int fileCount READ fileCount NOTIFY fileCountChanged)
    Q_PROPERTY(int seriesCount READ seriesCount NOTIFY seriesCountChanged)

public:
    explicit DicomController(QObject *parent = nullptr);

    int fileCount() const { return m_fileCount; }
    int seriesCount() const { return m_seriesCount; }
    Q_INVOKABLE void setRenderer(QObject* renderer);
    Q_INVOKABLE void startRender(const QString& folderPath);

signals:
    void fileCountChanged();
    void seriesCountChanged();

private:
    void runVolumePipeline(vtkImageData* data);

    int m_fileCount = 0;
    int m_seriesCount = 0;
    vtkSmartPointer<vtkRenderer> m_renderer;
    vtkSmartPointer<vtkRenderWindow> m_renderWindow;
    vtkSmartPointer<vtkDICOMReader> m_reader;
};

#endif
