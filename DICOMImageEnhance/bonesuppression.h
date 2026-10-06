#ifndef BONESUPPRESSION_H
#define BONESUPPRESSION_H

#include <QObject>
#include <QImage>
#include <QVector>
#include <QtMath>

class BoneSuppression : public QObject
{
    Q_OBJECT
public:
    explicit BoneSuppression(QObject *parent = nullptr);

    // Main processing function
    QVector<uint16_t> processImage(const QVector<uint16_t> &inputImage,
                                   int width, int height);

    // Alternative: process from raw file
    bool processRawFile(const QString &inputPath, const QString &outputPath);

private:
    // Simple bone suppression using edge detection and local contrast
    QVector<uint16_t> detectBoneEdges(const QVector<uint16_t> &image,
                                      int width, int height);

    // Apply Gaussian blur for soft tissue estimation
    QVector<float> gaussianBlur(const QVector<uint16_t> &image,
                                int width, int height, int radius);

    // Suppress bone regions
    QVector<uint16_t> suppressBones(const QVector<uint16_t> &original,
                                    const QVector<uint16_t> &boneEdges,
                                    float suppressionStrength);
};

#endif // BONESUPPRESSION_H
