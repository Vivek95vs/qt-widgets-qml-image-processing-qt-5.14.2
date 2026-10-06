#include <QCoreApplication>
#include <QFile>
#include <QImage>
#include <QDebug>
#include <cmath>
#include <cstring>

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    // ============ CONFIGURATION ============
        QString inputFile = "C:/input/image.raw";      // Change to your raw file path
        QString outputFile = "C:/output/processed.png"; // Change to your output path
        int imageSize = 1536;  // 1536x1536 image
        // ======================================

        qDebug() << "Loading raw image:" << inputFile;

        // Step 1: Open and read the raw file
        QFile file(inputFile);
        if (!file.open(QIODevice::ReadOnly)) {
            qDebug() << "ERROR: Cannot open file:" << inputFile;
            return 1;
        }

        int totalPixels = imageSize * imageSize;
        qint64 expectedSize = totalPixels * sizeof(unsigned short);

        if (file.size() < expectedSize) {
            qDebug() << "ERROR: File size too small. Expected:" << expectedSize << "bytes";
            file.close();
            return 1;
        }

        // Allocate buffer and read raw data
        unsigned short* subbuffer = new unsigned short[totalPixels];
        file.read((char*)subbuffer, expectedSize);
        file.close();

        qDebug() << "File loaded successfully. Total pixels:" << totalPixels;

        // Step 2: Find maximum value (your original logic)
        int max_value = 0;
        for(int m = 0; m < totalPixels; m++) {
            if(subbuffer[m] > max_value) {
                if(subbuffer[m] == 65535) {
                    // Skip 65535 values
                } else {
                    max_value = subbuffer[m];
                }
            }
        }

        if(max_value == 0) {
            max_value = 1;
        }

        qDebug() << "Maximum value:" << max_value;

        // Step 3: Process the image (your original processing)
        unsigned char* tempChar = new unsigned char[totalPixels];

        for(int i = 0; i < totalPixels; ++i) {
            float norm = (float)subbuffer[i] / max_value;
            float gammaCorrected = pow(norm, 0.5f);
            tempChar[i] = 255 - (unsigned char)(gammaCorrected * 255.0f);  // Inverse for bones white
        }

        // Step 4: Save as PNG
        QImage outputImage(imageSize, imageSize, QImage::Format_Grayscale8);
        memcpy(outputImage.bits(), tempChar, totalPixels);

        if (outputImage.save(outputFile, "PNG")) {
            qDebug() << "SUCCESS: Image saved to:" << outputFile;
        } else {
            qDebug() << "ERROR: Failed to save image to:" << outputFile;
            delete[] subbuffer;
            delete[] tempChar;
            return 1;
        }

        // Cleanup
        delete[] subbuffer;
        delete[] tempChar;

        qDebug() << "Processing complete!";

        return 0;
}
