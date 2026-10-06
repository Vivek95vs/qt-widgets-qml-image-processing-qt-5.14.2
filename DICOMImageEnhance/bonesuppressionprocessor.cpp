#include "bonesuppressionprocessor.h"
#include <QStandardPaths>
#include <QFile>
BoneSuppressionProcessor::BoneSuppressionProcessor(QObject *parent)
    : QObject(parent)
{
    m_outputPath = QStandardPaths::writableLocation(QStandardPaths::DesktopLocation)
                   + "/suppressed_output.raw";
}

void BoneSuppressionProcessor::loadImage(const QString &path)
{
    m_inputPath = path;

    // Load and verify image
    QFile file(m_inputPath);
    if (file.open(QIODevice::ReadOnly)) {
        QByteArray data = file.readAll();
        if (data.size() == 2560 * 3072 * sizeof(uint16_t)) {
            m_currentImage.resize(2560 * 3072);
            memcpy(m_currentImage.data(), data.data(), data.size());
            emit hasImageChanged();
        } else {
            emit processingError("Invalid image dimensions");
        }
        file.close();
    }
}

void BoneSuppressionProcessor::process()
{
    if (m_boneSuppression.processRawFile(m_inputPath, m_outputPath)) {
        emit processingFinished();
    } else {
        emit processingError("Failed to process image");
    }
}
