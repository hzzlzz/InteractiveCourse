#include "icserver.h"

#include <QTimer>
#include <QThreadPool>
#include "icmessagehandler.h"
#include "icmessage.h"
#include "icquestion.h"
#include "icsystemstore.h"
#include "icconfig.h"
#include "icmessageprocessor.h"

ICServer::ICServer(QObject *parent) :
    QObject(parent), settings(CONFIG_FILE, QSettings::IniFormat)
{
    udpSocket = new QUdpSocket(this);
    timer = new QTimer(this);
    handler = new ICMessageHandler(this);

    //connect(udpSocket,SIGNAL(connected()),this, SLOT(socketConnected()));
    // no effect currently
    //connect(udpSocket,SIGNAL(disconnected()),this, SLOT(socketClosed()));
    connect(udpSocket, SIGNAL(readyRead()), this, SLOT(processPendingDatagrams()));

    connect(timer, SIGNAL(timeout()), this, SLOT(endTest()));

    connect(handler, SIGNAL(serverDiscovery(QHostAddress,QString)), this, SLOT(processDiscovery(QHostAddress,QString)));
    connect(handler, SIGNAL(questionRequest(QHostAddress)), this, SLOT(processRequest(QHostAddress)));
    connect(handler, SIGNAL(answerReady(ICAnswer,QString)), this, SLOT(processAnswer(ICAnswer,QString)));
}

void ICServer::setIdentifier(QString identifier) { sid = identifier; }

bool ICServer::startTest(int duration)
{
    connect(handler, SIGNAL(connectionOffer(QString)), this, SLOT(processOffer(QString)));

    timer->setInterval(duration);
    timer->setSingleShot(true);

    testUdpSocket = new QUdpSocket(this);
    bool success = testUdpSocket->bind(CLIENT_PORT);
    if (!success) {
        delete testUdpSocket;
        return false;
    }

    ICMessage question = ICMessage::initWithCommonProperties();
    question.setProperty(settings.value(SID_CONFIG_TAG,DEFAULT_SID_TAG).toString(),sid);
    question.setProperty(settings.value(COMMAND_CONFIG_TAG,DEFAULT_CMD_TAG).toString(), QString::number(SERVER_DISCOVERY));
    // broadcast for three times
    for (int i = 0; i < 3; i++)
        testUdpSocket->writeDatagram(question.saveAsJson(), QHostAddress::Broadcast, SERVER_PORT);

    connect(testUdpSocket, SIGNAL(readyRead()), this, SLOT(processPendingDatagrams()));

    timer->start();
    return true;
}

void ICServer::endTest()
{
    disconnect(handler, SIGNAL(connectionOffer(QString)), this, SLOT(processOffer(QString)));
    disconnect(testUdpSocket, SIGNAL(readyRead()), this, SLOT(processPendingDatagrams()));

    testUdpSocket->close();
}

void ICServer::socketClosed(QAbstractSocket::SocketState state)
{
    if (state == QAbstractSocket::ClosingState)
    emit connectionClosed();
}

// listen on port 8027, broadcast question to 8026
bool ICServer::listen()
{
    return udpSocket->bind(SERVER_PORT);
}

// stop the server
void ICServer::stop()
{
    udpSocket->close();
}

void ICServer::processPendingDatagrams()
{
    QByteArray datagram;
    do {
        datagram.resize(udpSocket->pendingDatagramSize());
        udpSocket->readDatagram(datagram.data(), datagram.size());

        ICMessageProcessor processor(handler, datagram);
        QThreadPool::globalInstance()->start(&processor);

    } while (udpSocket->hasPendingDatagrams());
}

void ICServer::broadcastQuestion()
{
    ICQuestion currentQuestion = ICSystemStore::getInstance()->getCurrentQuestion();
    if (!currentQuestion.isNull()) {
        ICMessage question = ICMessage::initWithCommonProperties();
        question.setProperty(settings.value(SID_CONFIG_TAG,DEFAULT_SID_TAG).toString(),sid);
        question.setProperty(settings.value(COMMAND_CONFIG_TAG,DEFAULT_CMD_TAG).toString(), QString::number(QUESTION_DELIVER));

        udpSocket->writeDatagram(question.saveAsJson(), QHostAddress::Broadcast, CLIENT_PORT);
    }
}

// clientAddress requests a question
// make sure they run in sender's thread
void ICServer::processRequest(QHostAddress clientAddress)
{
    ICQuestion currentQuestion = ICSystemStore::getInstance()->getCurrentQuestion();
    if (currentQuestion.isNull()) {
        ICMessage not_questioning = ICMessage::initWithCommonProperties();
        not_questioning.setProperty(settings.value(SID_CONFIG_TAG,DEFAULT_SID_TAG).toString(),sid);
        not_questioning.setProperty(settings.value(COMMAND_CONFIG_TAG,DEFAULT_CMD_TAG).toString(), QString::number(NOT_QUESTIONING));

        udpSocket->writeDatagram(not_questioning.saveAsJson(), clientAddress, CLIENT_PORT);
    }
    else {
        ICMessage qusetionDeliver = ICMessage::initWithCommonProperties();
        qusetionDeliver.setProperty(settings.value(SID_CONFIG_TAG,DEFAULT_SID_TAG).toString(),sid);
        qusetionDeliver.setProperty(settings.value(COMMAND_CONFIG_TAG,DEFAULT_CMD_TAG).toString(), QString::number(QUESTION_DELIVER));
        QList<ICQuestion> questions;
        questions.append(currentQuestion);
        qusetionDeliver.setQuestions(questions);

        udpSocket->writeDatagram(qusetionDeliver.saveAsJson(), clientAddress, CLIENT_PORT);
    }
}

// a connection offer with identifier comes
void ICServer::processOffer(QString identifier)
{
    if (sid.compare(identifier) == 0) emit identifierCollision(sid);
}

// a server discovery with specified identifier comes
void ICServer::processDiscovery(QHostAddress clientAddress, QString identifier)
{
    if (sid.compare(identifier) == 0) {
        ICMessage connectionOffer = ICMessage::initWithCommonProperties();
        connectionOffer.setProperty(settings.value(SID_CONFIG_TAG,DEFAULT_SID_TAG).toString(),sid);
        connectionOffer.setProperty(settings.value(COMMAND_CONFIG_TAG,DEFAULT_CMD_TAG).toString(), QString::number(CONNECTION_OFFER));

        udpSocket->writeDatagram(connectionOffer.saveAsJson(), clientAddress, CLIENT_PORT);
    }
}

// an answer come with sender's uid
void ICServer::processAnswer(ICAnswer answer, QString uid)
{
    ICSystemStore *store = ICSystemStore::getInstance();
    ICQuestion currentQuestion = store->getCurrentQuestion();
    if (!currentQuestion.isNull() && currentQuestion.getId().compare(answer.getId()) == 0) {
        answer.setUid(uid);
        store->addAnswer(answer);
        emit newAnswerArrived(uid);
    }
}
