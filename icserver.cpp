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
    QObject(parent), settings(new QSettings(CONFIG_FILE, QSettings::IniFormat, this))
{
    isRunning = false;
}

void ICServer::setUp(QString identifier, QHostAddress address)
{
    sid = identifier;
    ipAddress = address;
}

QString ICServer::getIdentifier() const { return sid; }

bool ICServer::startSidTest(int duration)
{
    timer = new QTimer(this);

    connect(timer, SIGNAL(timeout()), this, SLOT(stopSidTest()));

    timer->setInterval(duration);
    timer->setSingleShot(true);

    testUdpSocket = new QUdpSocket(this);

    bool success = testUdpSocket->bind(ipAddress, CLIENT_PORT);
    // if bind failed
    if (!success) return false;

    connect(testUdpSocket, SIGNAL(readyRead()), this, SLOT(processPendingDatagrams()));

    ICMessage question = ICMessage::initWithCommonProperties();
    question.setProperty(settings->value(SID_CONFIG_TAG,DEFAULT_SID_TAG).toString(),sid);
    question.setProperty(settings->value(COMMAND_CONFIG_TAG,DEFAULT_CMD_TAG).toString(), QString::number(SERVER_DISCOVERY));

    // broadcast for three times
    for (int i = 0; i < 3; i++)
        testUdpSocket->writeDatagram(question.saveAsJson(), QHostAddress::Broadcast, SERVER_PORT);

    timer->start();
    return true;
}

void ICServer::stopSidTest()
{
    if (testUdpSocket != 0) {
        testUdpSocket->close();
    }
}

// not in use
void ICServer::socketClosed(QAbstractSocket::SocketError)
{
    emit connectionClosed();
}

// listen on port 8027, broadcast question to 8026
bool ICServer::listen()
{
    udpSocket = new QUdpSocket(this);

    if (udpSocket->bind(ipAddress, SERVER_PORT)) {
        connect(udpSocket, SIGNAL(readyRead()), this, SLOT(processPendingDatagrams()));
        //connect(udpSocket,SIGNAL(error(QAbstractSocket::SocketError)),this, SLOT(socketClosed(QAbstractSocket::SocketError)));
        isRunning = true;
        return true;
    } else return false;
}

// stop the server
void ICServer::stop()
{
    if (udpSocket != 0) {
        udpSocket->close();
        isRunning = false;
    }
}

void ICServer::processPendingDatagrams()
{
    QByteArray datagram;
    QUdpSocket *socket = isRunning ? udpSocket : testUdpSocket;

    do {
        QHostAddress *address = new QHostAddress;
        datagram.resize(socket->pendingDatagramSize());
        socket->readDatagram(datagram.data(), datagram.size(), address);

        ICMessageHandler *messageHandler = new ICMessageHandler(this);
        connect(messageHandler, SIGNAL(connectionOffer(QString)), this, SLOT(processSidCollision(QString)));
        connect(messageHandler, SIGNAL(serverDiscovery(QHostAddress,QString)), this, SLOT(processDiscovery(QHostAddress,QString)));
        connect(messageHandler, SIGNAL(questionRequest(QHostAddress)), this, SLOT(processRequest(QHostAddress)));
        connect(messageHandler, SIGNAL(answerReady(ICAnswer,QString)), this, SLOT(processAnswer(ICAnswer,QString)));

        ICMessageProcessor *processor = new ICMessageProcessor(messageHandler, *address, datagram);

        QThreadPool::globalInstance()->start(processor);

        qDebug()<< "[1] Received from " << *address << datagram;

        delete address;
    } while (socket->hasPendingDatagrams());
}

void ICServer::broadcastQuestion()
{
    ICQuestion currentQuestion = ICSystemStore::getInstance()->getCurrentQuestion();
    if (!currentQuestion.isNull()) {
        ICMessage question = ICMessage::initWithCommonProperties();
        question.setProperty(settings->value(SID_CONFIG_TAG,DEFAULT_SID_TAG).toString(),sid);
        question.setProperty(settings->value(COMMAND_CONFIG_TAG,DEFAULT_CMD_TAG).toString(), QString::number(QUESTION_DELIVER));
        QList<ICQuestion> questions;
        questions.append(currentQuestion);
        question.setQuestions(questions);

        udpSocket->writeDatagram(question.saveAsJson(), QHostAddress::Broadcast, CLIENT_PORT);
    }
}

// clientAddress requests a question
// make sure they run in sender's thread
void ICServer::processRequest(QHostAddress clientAddress)
{
    ICQuestion currentQuestion = ICSystemStore::getInstance()->getCurrentQuestion();
    ICMessage reply = ICMessage::initWithCommonProperties();
    if (currentQuestion.isNull()) {
        reply.setProperty(settings->value(SID_CONFIG_TAG,DEFAULT_SID_TAG).toString(),sid);
        reply.setProperty(settings->value(COMMAND_CONFIG_TAG,DEFAULT_CMD_TAG).toString(), QString::number(NOT_QUESTIONING));
    }
    else {
        reply.setProperty(settings->value(SID_CONFIG_TAG,DEFAULT_SID_TAG).toString(),sid);
        reply.setProperty(settings->value(COMMAND_CONFIG_TAG,DEFAULT_CMD_TAG).toString(), QString::number(QUESTION_DELIVER));
        QList<ICQuestion> questions;
        questions.append(currentQuestion);
        reply.setQuestions(questions);
    }

    udpSocket->writeDatagram(reply.saveAsJson(), clientAddress, CLIENT_PORT);

    qDebug() << "[2] Send to " << clientAddress << reply.saveAsJson() << " in thread " << QThread::currentThread();
}

// a connection offer with identifier comes
void ICServer::processSidCollision(QString identifier)
{
    if (sid.compare(identifier) == 0)
    {
        emit identifierCollision(sid);
    }
}

// a server discovery with specified identifier comes
void ICServer::processDiscovery(QHostAddress clientAddress, QString identifier)
{
    if (sid.compare(identifier) == 0) {
        ICMessage connectionOffer = ICMessage::initWithCommonProperties();
        connectionOffer.setProperty(settings->value(SID_CONFIG_TAG,DEFAULT_SID_TAG).toString(),sid);
        connectionOffer.setProperty(settings->value(COMMAND_CONFIG_TAG,DEFAULT_CMD_TAG).toString(), QString::number(CONNECTION_OFFER));

        udpSocket->writeDatagram(connectionOffer.saveAsJson(), clientAddress, CLIENT_PORT);

        qDebug() << "[3] Send to " << clientAddress << connectionOffer.saveAsJson() << " in thread " << QThread::currentThread();
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

        qDebug() << "[4] Answer from " << uid << "processed" << " in thread " << QThread::currentThread();
    }
}
