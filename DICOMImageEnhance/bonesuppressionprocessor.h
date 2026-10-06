#ifndef BONESUPPRESSIONPROCESSOR_H
#define BONESUPPRESSIONPROCESSOR_H

#include <QObject>
#include "bonesuppression.h"

class BoneSuppressionProcessor : public QObject
{
    Q_OBJECT
    Q_PROPERTY(bool hasImage READ hasImage NOTIFY hasImageChanged)

public:
    explicit BoneSuppressionProcessor(QObject *parent = nullptr);

    bool hasImage() const { return !m_currentImage.isEmpty(); }

public slots:
    void loadImage(const QString &path);
    void process();

signals:
    void hasImageChanged();
    void processingFinished();
    void processingError(const QString &error);

private:
    BoneSuppression m_boneSuppression;
    QString m_inputPath;
    QString m_outputPath;
    QVector<uint16_t> m_currentImage;
};

#endif
