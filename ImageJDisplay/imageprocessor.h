#ifndef IMAGEPROCESSOR_H
#define IMAGEPROCESSOR_H

#include <QImage>

class ImageProcessor
{
public:
    ImageProcessor();

    // Original source image
    void setSourceImage(const QImage &image);

    // Processing parameters
    void setMinimum(int value);
    void setMaximum(int value);
    void setBrightness(int value);
    void setContrast(double value);

    // Get processing parameters
    int minimum() const;
    int maximum() const;
    int brightness() const;
    double contrast() const;
    // auto


    // Reset parameters
    void reset();

    // Processing
    QImage processPreview() const;
    QImage processImage(const QImage &image) const;
    void setDataRange(double min, double max);


private:
    QImage process(const QImage &image) const;

private:
    QImage m_sourceImage;

    int m_minimum;
    int m_maximum;

    int m_brightness;
    double m_contrast;
    double m_dataMin;
    double m_dataMax;
};


#endif // IMAGEPROCESSOR_H
