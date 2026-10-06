#include "bonesuppression.h"
#include <QFile>
#include <QDebug>
#include <cmath>

BoneSuppression::BoneSuppression(QObject *parent) : QObject(parent)
{
}

QVector<uint16_t> BoneSuppression::processImage(const QVector<uint16_t> &inputImage,
                                                 int width, int height)
{
    if (inputImage.isEmpty() || width <= 0 || height <= 0)
        return QVector<uint16_t>();

    // Step 1: Detect bone edges using simple gradient
    QVector<uint16_t> boneEdges = detectBoneEdges(inputImage, width, height);

    // Step 2: Suppress bone regions
    float suppressionStrength = 0.7f; // 70% suppression
    return suppressBones(inputImage, boneEdges, suppressionStrength);
}

QVector<uint16_t> BoneSuppression::detectBoneEdges(const QVector<uint16_t> &image,
                                                    int width, int height)
{
    QVector<uint16_t> edges(width * height, 0);

    // Simple Sobel-like edge detection for bone boundaries
    for (int y = 1; y < height - 1; ++y) {
        for (int x = 1; x < width - 1; ++x) {
            int idx = y * width + x;

            // Simple gradient calculation
            int gx = qAbs(image[(y) * width + (x+1)] - image[(y) * width + (x-1)]);
            int gy = qAbs(image[(y+1) * width + x] - image[(y-1) * width + x]);

            int gradient = (gx + gy) / 2;

            // Threshold to identify bone edges (adjust based on your image range)
            if (gradient > 1000 && image[idx] > 8000) { // Typical bone intensity threshold
                edges[idx] = gradient;
            }
        }
    }

    return edges;
}

QVector<float> BoneSuppression::gaussianBlur(const QVector<uint16_t> &image,
                                              int width, int height, int radius)
{
    QVector<float> blurred(width * height, 0.0f);
    int kernelSize = 2 * radius + 1;
    QVector<float> kernel(kernelSize);

    // Simple Gaussian kernel approximation
    float sigma = radius / 2.0f;
    float sum = 0.0f;

    for (int i = -radius; i <= radius; ++i) {
        float value = qExp(-(i*i) / (2 * sigma * sigma));
        kernel[i + radius] = value;
        sum += value;
    }

    // Normalize kernel
    for (int i = 0; i < kernelSize; ++i) {
        kernel[i] /= sum;
    }

    // Horizontal blur
    QVector<float> temp(width * height, 0.0f);
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            float value = 0.0f;
            for (int k = -radius; k <= radius; ++k) {
                int nx = x + k;
                if (nx >= 0 && nx < width) {
                    value += image[y * width + nx] * kernel[k + radius];
                }
            }
            temp[y * width + x] = value;
        }
    }

    // Vertical blur
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            float value = 0.0f;
            for (int k = -radius; k <= radius; ++k) {
                int ny = y + k;
                if (ny >= 0 && ny < height) {
                    value += temp[ny * width + x] * kernel[k + radius];
                }
            }
            blurred[y * width + x] = value;
        }
    }

    return blurred;
}

QVector<uint16_t> BoneSuppression::suppressBones(const QVector<uint16_t> &original,
                                                  const QVector<uint16_t> &boneEdges,
                                                  float suppressionStrength)
{
    QVector<uint16_t> result(original.size());

    // Get soft tissue estimate using local average
    int windowSize = 15;
    int halfWindow = windowSize / 2;

    for (int y = 0; y < 3072; ++y) {
        for (int x = 0; x < 2560; ++x) {
            int idx = y * 2560 + x;

            if (boneEdges[idx] > 0) {
                // Calculate local average around bone edge
                long long sum = 0;
                int count = 0;

                for (int dy = -halfWindow; dy <= halfWindow; ++dy) {
                    for (int dx = -halfWindow; dx <= halfWindow; ++dx) {
                        int nx = x + dx;
                        int ny = y + dy;
                        if (nx >= 0 && nx < 2560 && ny >= 0 && ny < 3072) {
                            sum += original[ny * 2560 + nx];
                            count++;
                        }
                    }
                }

                if (count > 0) {
                    float localAvg = sum / (float)count;
                    // Suppress bone by blending with local soft tissue average
                    result[idx] = (uint16_t)(localAvg * suppressionStrength +
                                            original[idx] * (1.0f - suppressionStrength));
                } else {
                    result[idx] = original[idx];
                }
            } else {
                result[idx] = original[idx];
            }
        }
    }

    return result;
}

bool BoneSuppression::processRawFile(const QString &inputPath, const QString &outputPath)
{
    QFile inputFile(inputPath);
    if (!inputFile.open(QIODevice::ReadOnly)) {
        qDebug() << "Cannot open input file:" << inputPath;
        return false;
    }

    // Read 16-bit raw data (2560 * 3072 = 7,864,320 pixels)
    const int width = 2560;
    const int height = 3072;
    const int pixelCount = width * height;

    QVector<uint16_t> inputImage(pixelCount);

    // Read raw data
    QByteArray rawData = inputFile.readAll();
    if (rawData.size() != pixelCount * sizeof(uint16_t)) {
        qDebug() << "File size doesn't match expected dimensions";
        return false;
    }

    memcpy(inputImage.data(), rawData.data(), rawData.size());
    inputFile.close();

    // Process image
    QVector<uint16_t> outputImage = processImage(inputImage, width, height);

    // Save result
    QFile outputFile(outputPath);
    if (!outputFile.open(QIODevice::WriteOnly)) {
        qDebug() << "Cannot create output file:" << outputPath;
        return false;
    }

    outputFile.write(reinterpret_cast<const char*>(outputImage.data()),
                     outputImage.size() * sizeof(uint16_t));
    outputFile.close();

    return true;
}
