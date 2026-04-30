#ifndef CONTOURBACKEND_H
#define CONTOURBACKEND_H

#include <QObject>
#include <QString>
#include <QVariant>
#include <QVariantList>
#include <QColor>
#include <Python.h>

class ContourBackend : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString contourType READ contourType WRITE setContourType NOTIFY contourTypeChanged)
    Q_PROPERTY(QColor contourColor READ contourColor WRITE setContourColor NOTIFY contourColorChanged)
    Q_PROPERTY(QVariantList contourPoints READ contourPoints NOTIFY contourPointsChanged)
    Q_PROPERTY(double contourArea READ contourArea NOTIFY contourAreaChanged)
    Q_PROPERTY(QString statusMessage READ statusMessage NOTIFY statusMessageChanged)

public:
    explicit ContourBackend(QObject *parent = nullptr);
    ~ContourBackend();

    QString contourType() const { return m_contourType; }
    QColor contourColor() const { return m_contourColor; }
    QVariantList contourPoints() const { return m_contourPoints; }
    double contourArea() const { return m_contourArea; }
    QString statusMessage() const { return m_statusMessage; }

    void setContourType(const QString &type);
    void setContourColor(const QColor &color);

public slots:
    void generateContour(double size, double centerX, double centerY, double smoothing);
    void clearContour();
    void saveContourToFile(const QString &filename);
    void loadContourFromFile(const QString &filename);

signals:
    void contourTypeChanged(const QString &type);
    void contourColorChanged(const QColor &color);
    void contourPointsChanged(const QVariantList &points);
    void contourAreaChanged(double area);
    void statusMessageChanged(const QString &message);
    void contourGenerated(const QVariantList &points);

private:
    void initializePython();
    void finalizePython();
    QString callPythonFunction(const QString &function, const QVariantList &args);
    void parseContourData(const QString &jsonData);

    QString m_contourType;
    QColor m_contourColor;
    QVariantList m_contourPoints;
    double m_contourArea;
    QString m_statusMessage;

    PyObject *m_pythonModule;
    PyObject *m_pythonBackend;
    bool m_pythonInitialized;
};

#endif // CONTOURBACKEND_H
