#include "imageprocessor.h"

#include <QtGlobal>
#include <QtMath>
#include<debugtimer.h>
ImageProcessor::ImageProcessor()
    : m_minimum(0),
      m_maximum(65535),
      m_brightness(0),
      m_contrast(1.0),
      m_dataMin(0),
      m_dataMax(65535)
{
}

void ImageProcessor::setSourceImage(
        const QImage &image)
{
    m_sourceImage = image;
}
void ImageProcessor::setDataRange(double min, double max)
{
    m_dataMin = min;
    m_dataMax = max;
}

void ImageProcessor::setMinimum(int value)
{
    DebugTimer timer(__FUNCTION__);
    // Minimum: 0 -> 65535
    m_minimum = qBound(
        0,
        value,
        65534);

    // Maximum remains unchanged

    // Minimum 0 -> 65535
    // Brightness 0 -> -100
    const double ratio =
        m_minimum / 65535.0;

    m_brightness =
        qRound(-ratio * 100.0);

    // Contrast 1 -> 2
    m_contrast =
        1.0 + ratio;
}


void ImageProcessor::setMaximum(int value)
{
    DebugTimer timer(__FUNCTION__);
    // Maximum: 65535 -> 0
    m_maximum = qBound(
        1,
        value,
        65535);

    // Minimum remains unchanged

    // Maximum 65535 -> 0
    // Brightness 0 -> +100
    const double ratio =
        (65535.0 - m_maximum) / 65535.0;

    m_brightness =
        qRound(ratio * 100.0);

    // Contrast 1 -> 2
    m_contrast =
        1.0 + ratio;
}




void ImageProcessor::setBrightness(int value)
{
    DebugTimer timer(__FUNCTION__);
    m_brightness = qBound(
        -100,
        value,
        100);

    const double ratio =
        m_brightness / 100.0;

    if (m_brightness > 0)
    {
        // Brightness: 0 -> +100
        // Maximum: 65535 -> 32767.5
        // Minimum unchanged

        m_maximum = qRound(
            65535.0 -
            ratio * (65534.0 / 2.0));
    }
    else if (m_brightness < 0)
    {
        // Brightness: 0 -> -100
        // Minimum: 0 -> 32767.5
        // Maximum unchanged

        m_minimum = qRound(
            (-ratio) * (65534.0 / 2.0));
    }
    else
    {
        // Brightness = 0
        // Min/Max ko change nahi karna
    }

    // Contrast unchanged
}
void ImageProcessor::setContrast(double value)
{
    DebugTimer timer(__FUNCTION__);
    m_contrast = qBound(
        0.0,
        value,
        2.0);

    // Contrast: 1.0 -> 2.0
    if (m_contrast > 1.0)
    {
        const double ratio =
            m_contrast - 1.0;

        // Minimum: 0 -> 32767.5
        m_minimum = qRound(
            ratio * (65534.0 / 2.0));

        // Maximum: 65535 -> 32767.5
        m_maximum = qRound(
            65535.0 -
            ratio * (65534.0 / 2.0));
    }

    // Contrast: 1.0 -> 0.0
    else if (m_contrast < 1.0)
    {
        // Min/Max unchanged
        // Brightness changes only slightly

        const double ratio =
            1.0 - m_contrast;

        m_brightness = qRound(
            ratio * 10.0);
    }

    // Contrast = 1.0
    else
    {
        // Min/Max/Brightness unchanged
    }
}


int ImageProcessor::minimum() const
{
    return m_minimum;
}

int ImageProcessor::maximum() const
{
    return m_maximum;
}

int ImageProcessor::brightness() const
{
    return m_brightness;
}

double ImageProcessor::contrast() const
{
    return m_contrast;
}

void ImageProcessor::reset()
{
    m_minimum = 0;
    m_maximum = 65535;

    m_brightness = 0.0;
    m_contrast = 1.0;


}

QImage ImageProcessor::processPreview() const
{
    if (m_sourceImage.isNull())
        return QImage();

    return process(
                m_sourceImage);
}

QImage ImageProcessor::processImage(
        const QImage &image) const
{
    if (image.isNull())
        return QImage();

    return process(image);
}

QImage ImageProcessor::process(
        const QImage &image) const
{
    DebugTimer timer(__FUNCTION__);
    if (image.isNull())
        return QImage();

    if (image.format() != QImage::Format_Grayscale16)
        return QImage();

    const int width = image.width();
    const int height = image.height();

    QImage result(
        image.size(),
        QImage::Format_Grayscale16);

    const double range =
        static_cast<double>(m_maximum - m_minimum);

    if (range <= 0.0)
        return image;
    QElapsedTimer pixelTimer;
    pixelTimer.start();
    for (int y = 0; y < height; ++y)
    {
        const quint16 *sourceLine =
            reinterpret_cast<const quint16 *>(
                image.constScanLine(y));

        quint16 *resultLine =
            reinterpret_cast<quint16 *>(
                result.scanLine(y));

        for (int x = 0; x < width; ++x)
        {
            const double pixel =
                static_cast<double>(sourceLine[x]);

            // =================================
            // MIN / MAX WINDOW
            // =================================
            double normalized =
                (pixel - m_minimum)
                * 65535.0
                / range;

            // =================================
            // FINAL CLAMP
            // =================================
            normalized =
                qBound(
                    0.0,
                    normalized,
                    65535.0);

            resultLine[x] =
                static_cast<quint16>(
                    qRound(normalized));
        }
    }
    qDebug() << "[TIMER] Pixel processing:"
             << pixelTimer.elapsed()
             << "ms";

    return result;
}
