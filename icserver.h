#ifndef ICSERVER_H
#define ICSERVER_H

#include <QObject>
#include <QUdpSocket>
#include <QSettings>
#include <QTimer>

#include "icmessagehandler.h"

class QHostAddress;
class ICQuestion;
class ICAnswer;

class ICServer : public QObject
{
    Q_OBJECT
public:
    explicit ICServer(QObject *parent = 0);

    void setUp(QString identifier, QHostAddress address = QHostAddress());
    // broadcast identifier to 8027, listen on port 8026
    // you need to connect identifier collision signal. 3000ms
    bool startSidTest(int duration = 3000);
    // listen on port 8027, broadcast question to 8026
    bool listen();
    // stop the server
    void stop();

    // need to set current question first in store object
    void broadcastQuestion();

    QString getIdentifier() const;

signals:
    void identifierCollision(QString identifer);
    void newAnswerArrived(QString uid);
    void connectionClosed();

public slots:
    // clientAddress requests a question
    void processRequest(QHostAddress clientAddress);
    // a connection offer with identifier comes
    void processSidCollision(QString identifier);
    // a server discovery with specified identifier comes
    void processDiscovery(QHostAddress clientAddress, QString identifier);
    // an answer come with sender's uid
    void processAnswer(ICAnswer answer, QString uid);

private slots:
    void socketClosed(QAbstractSocket::SocketError);

    void stopSidTest();

    void processPendingDatagrams();

private:
    QSettings *settings;
    QHostAddress ipAddress;
    // whether this server is serving connection
    bool isRunning;
    QUdpSocket *udpSocket;
    QUdpSocket *testUdpSocket;
    QTimer *timer;
    QString sid;
};

#endif // ICSERVER_H
