#ifndef ICMESSAGEHANDLER_H
#define ICMESSAGEHANDLER_H

#include "handler.h"

#include <QObject>
#include <QHostAddress>
#include <QSettings>
#include "icanswer.h"

class ICQuestion;

class ICMessageHandler : public Handler
{
    Q_OBJECT
public:
    ICMessageHandler(QObject *parent = 0);

    virtual void reset(const QList<QVariant>& paramList);
    virtual void done();
    
signals:
    // clientAddress requests a question
    void questionRequest(QHostAddress clientAddress);
    // a connection offer with identifier comes
    void connectionOffer(QString identifier);
    // a server discovery with specified identifier comes
    void serverDiscovery(QHostAddress clientAddress, QString identifier);
    // an answer come with sender's uid
    void answerReady(ICAnswer answer, QString uid);

public slots:
    void handle_app(QVariant app);
    void handle_version(QVariant version);
    void handle_sid(QVariant sid);
    void handle_uid(QVariant uid);
    void handle_cmd(QVariant command);
    void handle_questions(QVariant questions);
    void handle_answers(QVariant answers);
    void handle_senderip(QVariant senderip);

private:
    QSettings settings;

    bool appPass;
    bool versionPass;

    QString sid;
    QString uid;
    QString cmd;
    QList<ICAnswer> answers;

    QHostAddress clientAddress;
};

#endif // ICMESSAGEHANDLER_H
