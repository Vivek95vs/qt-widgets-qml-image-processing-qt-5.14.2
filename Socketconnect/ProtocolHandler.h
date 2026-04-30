#ifndef PROTOCOLHANDLER_H
#define PROTOCOLHANDLER_H

#include <QObject>
#include <QThread>
#include <QTimer>
#include <QFile>
#include <QDir>
#include <QDateTime>
#include "SocketClient.h"
#include <functional> // For std::function

class ProtocolHandler : public QObject
{
    Q_OBJECT
    Q_PROPERTY(SocketClient* client READ client WRITE setClient NOTIFY clientChanged)

    // Expose display counts to QML
    Q_PROPERTY(int displayCount1 READ displayCount1 NOTIFY displayCountsChanged)
    Q_PROPERTY(int displayCount2 READ displayCount2 NOTIFY displayCountsChanged)
    Q_PROPERTY(int displayCount3 READ displayCount3 NOTIFY displayCountsChanged)
    Q_PROPERTY(int displayCount4 READ displayCount4 NOTIFY displayCountsChanged)
    Q_PROPERTY(int displayCount5 READ displayCount5 NOTIFY displayCountsChanged)
    Q_PROPERTY(int displayCount6 READ displayCount6 NOTIFY displayCountsChanged)
    Q_PROPERTY(int displayCount7 READ displayCount7 NOTIFY displayCountsChanged)
    Q_PROPERTY(QString errorCodes READ errorCodes NOTIFY errorCodesChanged)

public:
    explicit ProtocolHandler(QObject *parent = nullptr);
    ~ProtocolHandler();

    SocketClient* client() const;
    void setClient(SocketClient *client);

    // Property getters
    int displayCount1() const { return m_displayCounts[0]; }
    int displayCount2() const { return m_displayCounts[1]; }
    int displayCount3() const { return m_displayCounts[2]; }
    int displayCount4() const { return m_displayCounts[3]; }
    int displayCount5() const { return m_displayCounts[4]; }
    int displayCount6() const { return m_displayCounts[5]; }
    int displayCount7() const { return m_displayCounts[6]; }
    QString errorCodes() const;

    Q_INVOKABLE void bootParameters();
    Q_INVOKABLE void startThreads();
    Q_INVOKABLE void stopThreads();

signals:
    void clientChanged();
    void feedbackUpdated(const QString &message);
    void displayCountsChanged();
    void errorCodesChanged();
    void connectionStatusChanged(bool connected);

private slots:
    void feedbackDisplay();
    void processReceivedData(const QByteArray &data);
    void attemptReconnect();

private:
    void communicationProcessData(const QByteArray &statusBuffer);
    void encoderCountsToDistance();
    void logConnectionEvent(const QString &message);

    SocketClient *m_client;
    QThread m_feedbackThread;
    QTimer m_feedbackTimer;
    QTimer m_reconnectTimer;
    bool m_running;
    bool m_communicationEstablished;
    bool m_eventTxRxTrigger;
    bool m_dummyTxStatus;
    int m_reconnectFlag;
    int m_connectDebounce;
    int m_logOtf;

    // Data storage
    int m_displayCounts[7];
    int m_errorCodes[7];
    int m_digitalInputFeedback[7];
    double m_currentEncoderDistance[7];
    QString m_communicationLogPath;

    // Constants (should be moved to configuration)
    static const int PULSE_PER_ROTATION[7];
    static const double LINEAR_DISTANCE_PER_ROTATION[7];
};

#endif // PROTOCOLHANDLER_H
