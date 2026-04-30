#ifndef ERRORHANDLER_H
#define ERRORHANDLER_H

#include <QObject>
#include <QVector>
#include <QString>

struct ErrorInfo {
    int code;
    QString description;
    QString details;
    bool isError;
};

class ErrorHandler : public QObject
{
    Q_OBJECT
public:
    explicit ErrorHandler(QObject *parent = nullptr);

    QVector<ErrorInfo> parseErrorCodes(const QByteArray &data);
    QString errorCodeToString(int code);

signals:
    void errorsUpdated(const QVector<ErrorInfo> &errors);

public slots:
    void processErrorData(const QByteArray &data);

private:
    QVector<ErrorInfo> m_currentErrors;
};

#endif // ERRORHANDLER_H
