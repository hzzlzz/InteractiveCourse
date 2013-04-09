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

    void setIdentifier(QString identifier);
    // broadcast identifier to 8027, listen on port 8026
    // you need to connect identifier collision signal. 3000ms
    bool startTest(int duration = 3000);
    // listen on port 8027, broadcast question to 8026
    bool listen();
    // stop the server
    void stop();

    // need to set current question first in store object
    void broadcastQuestion();

signals:
    void identifierCollision(QString identifer);
    void newAnswerArrived(QString uid);
    void connectionClosed();
public slots:
    // clientAddress requests a question
    void processRequest(QHostAddress clientAddress);
    // a connection offer with identifier comes
    void processOffer(QString identifier);
    // a server discovery with specified identifier comes
    void processDiscovery(QHostAddress clientAddress, QString identifier);
    // an answer come with sender's uid
    void processAnswer(ICAnswer answer, QString uid);

private slots:
    void socketClosed(QAbstractSocket::SocketState state);

    void endTest();

    void processPendingDatagrams();

private:
    QSettings settings;
    QUdpSocket *udpSocket;
    QUdpSocket *testUdpSocket;
    QTimer *timer;
    QString sid;
    ICMessageHandler *handler;
};

#endif // ICSERVER_H
