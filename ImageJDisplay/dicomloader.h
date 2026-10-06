#ifndef DICOMLOADER_H
#define DICOMLOADER_H

#include "imageprocessor.h"

#include <QObject>
#include <QString>
#include <QImage>
#include <QVariantList>
class DicomLoader : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QString filePath
               READ filePath
               NOTIFY filePathChanged)

    Q_PROPERTY(QString patientName
               READ patientName
               NOTIFY metadataChanged)

    Q_PROPERTY(QString patientId
               READ patientId
               NOTIFY metadataChanged)

    Q_PROPERTY(QString studyDescription
               READ studyDescription
               NOTIFY metadataChanged)

    Q_PROPERTY(QString seriesDescription
               READ seriesDescription
               NOTIFY metadataChanged)
    Q_PROPERTY(QString patientBirthDate
               READ patientBirthDate
               NOTIFY metadataChanged)

    Q_PROPERTY(QString patientSex
               READ patientSex
               NOTIFY metadataChanged)

    Q_PROPERTY(QString modality
               READ modality
               NOTIFY metadataChanged)

    Q_PROPERTY(QString studyDate
               READ studyDate
               NOTIFY metadataChanged)

    Q_PROPERTY(int imageWidth
               READ imageWidth
               NOTIFY imageChanged)

    Q_PROPERTY(int imageHeight
               READ imageHeight
               NOTIFY imageChanged)

    Q_PROPERTY(int imageRevision
               READ imageRevision
               NOTIFY imageChanged)

    Q_PROPERTY(bool loaded
               READ isLoaded
               NOTIFY loadedChanged)

    // // for windows level, width
    Q_PROPERTY(int minimum
               READ minimum
               NOTIFY imageProcessingChanged)

    Q_PROPERTY(int maximum
               READ maximum
               NOTIFY imageProcessingChanged)

    Q_PROPERTY(int brightness
               READ brightness
               NOTIFY imageProcessingChanged)

    Q_PROPERTY(double contrast
               READ contrast
               NOTIFY imageProcessingChanged)

    Q_PROPERTY(bool imageProcessingApplied
               READ imageProcessingApplied
               NOTIFY imageProcessingChanged)

    //Histogram

    // =====================================================
    // HISTOGRAM
    // =====================================================

    Q_PROPERTY(QVariantList histogramData
               READ histogramData
               NOTIFY histogramChanged)

    Q_PROPERTY(qulonglong histogramCount
               READ histogramCount
               NOTIFY histogramChanged)

    Q_PROPERTY(double histogramMean
               READ histogramMean
               NOTIFY histogramChanged)

    Q_PROPERTY(double histogramStdDev
               READ histogramStdDev
               NOTIFY histogramChanged)

    Q_PROPERTY(int histogramMin
               READ histogramMin
               NOTIFY histogramChanged)

    Q_PROPERTY(int histogramMax
               READ histogramMax
               NOTIFY histogramChanged)

    Q_PROPERTY(int histogramMode
               READ histogramMode
               NOTIFY histogramChanged)

    Q_PROPERTY(qulonglong histogramModeCount
               READ histogramModeCount
               NOTIFY histogramChanged)

    Q_PROPERTY(int histogramBins
               READ histogramBins
               NOTIFY histogramChanged)

    Q_PROPERTY(double histogramBinWidth
               READ histogramBinWidth
               NOTIFY histogramChanged)

    // Status
    Q_PROPERTY(QString statusText
               READ statusText
               NOTIFY statusChanged)

    Q_PROPERTY(QString statusType
               READ statusType
               NOTIFY statusChanged)

    Q_PROPERTY(bool modified
               READ isModified
               NOTIFY modifiedChanged)

public:

    explicit DicomLoader(QObject *parent = nullptr);

    QString filePath() const;

    QString patientName() const;
    QString patientId() const;

    QString studyDescription() const;
    QString seriesDescription() const;

    QString patientBirthDate() const;
    QString patientSex() const;
    QString modality() const;
    QString studyDate() const;
    QImage previewImage() const;

    int imageWidth() const;
    int imageHeight() const;

    int imageRevision() const;

    bool isLoaded() const;

    QImage image() const;

    // Called directly from QML
    Q_INVOKABLE bool loadDicom(const QString &filePath);
    // Save processed DICOM
    Q_INVOKABLE bool saveProcessedDicom(const QString &filePath);
    bool imageProcessingApplied() const;

    Q_INVOKABLE void clear();

    // for windows level, width
    Q_INVOKABLE void setMinimum(int value);
    Q_INVOKABLE void setMaximum(int value);
    Q_INVOKABLE void setBrightness(int value);
    Q_INVOKABLE void setContrast(double value);
    //reset
    Q_INVOKABLE void resetImageProcessing();
    // auto
    Q_INVOKABLE void autoImageProcessing();
    //apply
    Q_INVOKABLE void applyImageProcessing();

    //Status
    QString statusText() const;
    QString statusType() const;

    bool isModified() const;

    //----




    int minimum() const;
    int maximum() const;
    int brightness() const;
    double contrast() const;

    // =====================================================
    // HISTOGRAM
    // =====================================================

    Q_INVOKABLE void calculateHistogram();

    QVariantList histogramData() const;

    qulonglong histogramCount() const;

    double histogramMean() const;

    double histogramStdDev() const;

    int histogramMin() const;

    int histogramMax() const;

    int histogramMode() const;

    qulonglong histogramModeCount() const;

    int histogramBins() const;

    double histogramBinWidth() const;
    Q_INVOKABLE void clearHistogram();

    //............

signals:

    void filePathChanged();

    void metadataChanged();

    void imageChanged();

    void loadedChanged();

    void errorOccurred(const QString &message);

    void imageProcessingChanged();

    void histogramChanged();
    //Status
    void statusChanged();
    void modifiedChanged();

private:

    void clearData(bool emitSignals = true);
    void updatePreview();

private:

    QString m_filePath;

    QString m_patientName;
    QString m_patientId;

    QString m_studyDescription;
    QString m_seriesDescription;

    QString m_patientBirthDate;
    QString m_patientSex;
    QString m_modality;
    QString m_studyDate;

    QImage m_image;

    int m_imageRevision;


    ImageProcessor m_imageProcessor;

    QImage m_previewImage;
    // for apply
    QImage m_processedImage;

     // Loaded state
    bool m_loaded;

    // True only after full-resolution APPLY
    bool m_imageProcessingApplied;

    // =====================================================
    // HISTOGRAM DATA
    // =====================================================

    QVariantList m_histogramData;

    qulonglong m_histogramCount;

    double m_histogramMean;

    double m_histogramStdDev;
    int m_bitsAllocated;
    int m_histogramMin;

    int m_histogramMax;

    int m_histogramMode;

    qulonglong m_histogramModeCount;

    int m_histogramBins;

    double m_histogramBinWidth;

    //Status
    QString m_statusText = "No Image";
    QString m_statusType = "idle";

    bool m_modified = false;
    void setStatus(
            const QString &text,
            const QString &type);

};



#endif // DICOMLOADER_H
