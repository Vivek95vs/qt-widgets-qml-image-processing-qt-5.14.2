#include "contourbackend.h"
#include <QDebug>
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QDir>

ContourBackend::ContourBackend(QObject *parent)
    : QObject(parent)
    , m_contourType("circle")
    , m_contourColor(Qt::red)
    , m_contourArea(0.0)
    , m_pythonModule(nullptr)
    , m_pythonBackend(nullptr)
    , m_pythonInitialized(false)
{
    initializePython();
}

ContourBackend::~ContourBackend()
{
    finalizePython();
}

void ContourBackend::initializePython()
{
    try {
        Py_Initialize();

        // Add current directory to Python path
        PyRun_SimpleString("import sys");
        PyRun_SimpleString("sys.path.append('.')");

        // Import our Python module
        m_pythonModule = PyImport_ImportModule("PythonBackend");
        if (!m_pythonModule) {
            PyErr_Print();
            m_statusMessage = "Failed to import Python module";
            emit statusMessageChanged(m_statusMessage);
            return;
        }

        // Create PythonBackend instance
        PyObject *backendClass = PyObject_GetAttrString(m_pythonModule, "PythonBackend");
        if (!backendClass) {
            PyErr_Print();
            m_statusMessage = "Failed to get PythonBackend class";
            emit statusMessageChanged(m_statusMessage);
            return;
        }

        m_pythonBackend = PyObject_CallObject(backendClass, nullptr);
        Py_DECREF(backendClass);

        if (!m_pythonBackend) {
            PyErr_Print();
            m_statusMessage = "Failed to create PythonBackend instance";
            emit statusMessageChanged(m_statusMessage);
            return;
        }

        m_pythonInitialized = true;
        m_statusMessage = "Python backend initialized successfully";
        emit statusMessageChanged(m_statusMessage);

    } catch (...) {
        m_statusMessage = "Exception during Python initialization";
        emit statusMessageChanged(m_statusMessage);
    }
}

void ContourBackend::finalizePython()
{
    if (m_pythonBackend) {
        Py_DECREF(m_pythonBackend);
        m_pythonBackend = nullptr;
    }

    if (m_pythonModule) {
        Py_DECREF(m_pythonModule);
        m_pythonModule = nullptr;
    }

    if (Py_IsInitialized()) {
        Py_Finalize();
    }

    m_pythonInitialized = false;
}

QString ContourBackend::callPythonFunction(const QString &function, const QVariantList &args)
{
    if (!m_pythonInitialized || !m_pythonBackend) {
        return "{\"error\": \"Python not initialized\"}";
    }

    PyObject *pyFunc = PyObject_GetAttrString(m_pythonBackend, function.toUtf8().constData());
    if (!pyFunc || !PyCallable_Check(pyFunc)) {
        PyErr_Print();
        return QString("{\"error\": \"Function %1 not found or not callable\"}").arg(function);
    }

    // Prepare arguments
    PyObject *pyArgs = PyTuple_New(args.size());
    for (int i = 0; i < args.size(); ++i) {
        QVariant arg = args[i];
        if (arg.type() == QVariant::String) {
            PyTuple_SetItem(pyArgs, i, PyUnicode_FromString(arg.toString().toUtf8().constData()));
        } else if (arg.type() == QVariant::Double) {
            PyTuple_SetItem(pyArgs, i, PyFloat_FromDouble(arg.toDouble()));
        } else if (arg.type() == QVariant::Int) {
            PyTuple_SetItem(pyArgs, i, PyLong_FromLong(arg.toInt()));
        } else {
            PyTuple_SetItem(pyArgs, i, Py_None);
            Py_INCREF(Py_None);
        }
    }

    // Call function
    PyObject *pyResult = PyObject_CallObject(pyFunc, pyArgs);
    Py_DECREF(pyArgs);
    Py_DECREF(pyFunc);

    if (!pyResult) {
        PyErr_Print();
        return "{\"error\": \"Python function call failed\"}";
    }

    // Convert result to QString
    PyObject *pyStr = PyObject_Str(pyResult);
    QString result = QString::fromUtf8(PyUnicode_AsUTF8(pyStr));
    Py_DECREF(pyStr);
    Py_DECREF(pyResult);

    return result;
}

void ContourBackend::parseContourData(const QString &jsonData)
{
    QJsonDocument doc = QJsonDocument::fromJson(jsonData.toUtf8());
    if (doc.isNull() || !doc.isObject()) {
        m_statusMessage = "Failed to parse contour data";
        emit statusMessageChanged(m_statusMessage);
        return;
    }

    QJsonObject obj = doc.object();

    if (obj.contains("error")) {
        m_statusMessage = "Python error: " + obj["error"].toString();
        emit statusMessageChanged(m_statusMessage);
        return;
    }

    // Parse points
    m_contourPoints.clear();
    QJsonArray pointsArray = obj["points"].toArray();
    for (const QJsonValue &pointVal : pointsArray) {
        QJsonArray pointArray = pointVal.toArray();
        if (pointArray.size() >= 2) {
            QVariantList point;
            point << pointArray[0].toDouble();
            point << pointArray[1].toDouble();
            m_contourPoints.append(point);
        }
    }

    // Calculate area using Python
    QVariantList areaArgs;
    areaArgs << jsonData;
    QString areaResult = callPythonFunction("calculate_area", QVariantList());
    QJsonDocument areaDoc = QJsonDocument::fromJson(areaResult.toUtf8());
    if (!areaDoc.isNull() && areaDoc.isObject()) {
        QJsonObject areaObj = areaDoc.object();
        if (areaObj.contains("area")) {
            m_contourArea = areaObj["area"].toDouble();
        }
    }

    emit contourPointsChanged(m_contourPoints);
    emit contourAreaChanged(m_contourArea);
    m_statusMessage = QString("Contour generated: %1 points, Area: %2")
                      .arg(m_contourPoints.size())
                      .arg(m_contourArea, 0, 'f', 2);
    emit statusMessageChanged(m_statusMessage);
}

void ContourBackend::setContourType(const QString &type)
{
    if (m_contourType != type) {
        m_contourType = type;
        emit contourTypeChanged(m_contourType);
    }
}

void ContourBackend::setContourColor(const QColor &color)
{
    if (m_contourColor != color) {
        m_contourColor = color;
        emit contourColorChanged(m_contourColor);
    }
}

void ContourBackend::generateContour(double size, double centerX, double centerY, double smoothing)
{
    if (!m_pythonInitialized) {
        m_statusMessage = "Python backend not initialized";
        emit statusMessageChanged(m_statusMessage);
        return;
    }

    QVariantList args;
    args << m_contourType;
    args << centerX;
    args << centerY;
    args << size;

    QString result = callPythonFunction("generate_contour", args);
    parseContourData(result);

    // Apply smoothing if needed
    if (smoothing > 0) {
        QVariantList smoothArgs;
        smoothArgs << result;
        smoothArgs << smoothing;
        QString smoothResult = callPythonFunction("smooth_current_contour", smoothArgs);
        parseContourData(smoothResult);
    }

    emit contourGenerated(m_contourPoints);
}

void ContourBackend::clearContour()
{
    m_contourPoints.clear();
    m_contourArea = 0.0;
    emit contourPointsChanged(m_contourPoints);
    emit contourAreaChanged(m_contourArea);
    m_statusMessage = "Contour cleared";
    emit statusMessageChanged(m_statusMessage);
}

void ContourBackend::saveContourToFile(const QString &filename)
{
    QFile file(filename);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        m_statusMessage = "Failed to open file for writing";
        emit statusMessageChanged(m_statusMessage);
        return;
    }

    QJsonObject contourObj;
    contourObj["type"] = m_contourType;
    contourObj["color"] = m_contourColor.name();
    contourObj["area"] = m_contourArea;

    QJsonArray pointsArray;
    for (const QVariant &pointVar : m_contourPoints) {
        QVariantList point = pointVar.toList();
        QJsonArray pointArray;
        pointArray << point[0].toDouble();
        pointArray << point[1].toDouble();
        pointsArray.append(pointArray);
    }
    contourObj["points"] = pointsArray;

    QJsonDocument doc(contourObj);
    file.write(doc.toJson());
    file.close();

    m_statusMessage = QString("Contour saved to %1").arg(filename);
    emit statusMessageChanged(m_statusMessage);
}

void ContourBackend::loadContourFromFile(const QString &filename)
{
    QFile file(filename);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        m_statusMessage = "Failed to open file for reading";
        emit statusMessageChanged(m_statusMessage);
        return;
    }

    QByteArray data = file.readAll();
    file.close();

    QJsonDocument doc = QJsonDocument::fromJson(data);
    if (doc.isNull() || !doc.isObject()) {
        m_statusMessage = "Failed to parse contour file";
        emit statusMessageChanged(m_statusMessage);
        return;
    }

    QJsonObject obj = doc.object();

    setContourType(obj["type"].toString());
    setContourColor(QColor(obj["color"].toString()));
    m_contourArea = obj["area"].toDouble();

    m_contourPoints.clear();
    QJsonArray pointsArray = obj["points"].toArray();
    for (const QJsonValue &pointVal : pointsArray) {
        QJsonArray pointArray = pointVal.toArray();
        if (pointArray.size() >= 2) {
            QVariantList point;
            point << pointArray[0].toDouble();
            point << pointArray[1].toDouble();
            m_contourPoints.append(point);
        }
    }

    emit contourPointsChanged(m_contourPoints);
    emit contourAreaChanged(m_contourArea);
    m_statusMessage = QString("Contour loaded from %1").arg(filename);
    emit statusMessageChanged(m_statusMessage);
}
