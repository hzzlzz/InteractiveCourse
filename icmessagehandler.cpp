#include "icmessagehandler.h"

#include <QVariant>
#include <QThread>
#include <QMetaType>
#include "icconfig.h"

Q_DECLARE_METATYPE(QHostAddress)

ICMessageHandler::ICMessageHandler(QObject *parent) :
    Handler(parent), settings(CONFIG_FILE, QSettings::IniFormat)
{
}

void ICMessageHandler::reset(const QList<QVariant>& paramList)
{
    appPass = false;
    versionPass = false;
    sid = QString();
    uid = QString();
    cmd = QString();
    answers = QList<ICAnswer>();

    QVariant v = paramList.value(0);
    if (v.canConvert<QHostAddress>())
        clientAddress = v.value<QHostAddress>();

    qDebug() << "[101] Reset in thread " << QThread::currentThread();
}

void ICMessageHandler::done()
{
    if (!appPass || !versionPass) return;
    if (cmd.isNull()) {
        qDebug() << "Protocol Error: no cmd received";
        return;
    }

    switch(cmd.toInt()) {
    // connection offer comes
    case CONNECTION_OFFER:
        if (sid.isNull()) qDebug() << "Protocol Error: no sid in cmd " << cmd;
        else {
            emit connectionOffer(sid);
        }
        break;
    // server discovery comes
    case SERVER_DISCOVERY:
        if (sid.isNull()) qDebug() << "Protocol Error: no sid in cmd " << cmd;
        else {
            if (clientAddress.isNull()) qDebug() << "Application Error: client address not set";
            else emit serverDiscovery(clientAddress, sid);
        }
        break;
    // question request comes
    case QUESTION_REQUEST:
        if (clientAddress.isNull()) qDebug() << "Application Error: client address not set";
        else emit questionRequest(clientAddress);
        break;
    // answer comes
    case ANSWER:
        if (uid.isNull()) qDebug() << "Protocol Error: no uid found in cmd " << cmd;
        else {
            if (answers.isEmpty()) qDebug() << "[106] Protocol Error: no answers found in cmd " << cmd;
            else {
                foreach (const ICAnswer& answer, answers) {
                    emit answerReady(answer, uid);
                }
            }
        }
        break;
    default:
        qDebug() << "Protocol Error: incorrect cmd " << cmd;
        break;
    }

    qDebug() << "[102] Done in thread " << QThread::currentThread();
}

void ICMessageHandler::handle_app(QVariant app_v)
{
    if (app_v.canConvert<QString>()) {
        QString app = app_v.toString();
        if (app.compare(settings.value(APP_CONFIG_VALUE, DEFAULT_APP_VALUE).toString()) == 0) {
            appPass = true;
        } else qDebug() << "Protocol Error: incorrect app name " << app;
    } else qDebug() << "Application Error: app cannot convert to a qstring";
}

void ICMessageHandler::handle_version(QVariant version_v)
{
    if (version_v.canConvert<QString>()) {
        QString version = version_v.toString();
        if (version.compare(settings.value(VERSION_CONFIG_VALUE,DEFAULT_VERSION_VALUE).toString()) == 0) {
            versionPass = true;
        } else qDebug() << "Protocol Error: incompatible version " << version;
    } else qDebug() << "Application Error: version cannot convert to a qstring";
}

void ICMessageHandler::handle_sid(QVariant sid_v) { sid = sid_v.toString(); }
void ICMessageHandler::handle_uid(QVariant uid_v) { uid = uid_v.toString(); }
void ICMessageHandler::handle_cmd(QVariant command) { cmd = command.toString(); }

void ICMessageHandler::handle_questions(QVariant questions)
{
    // I don't handle questions
    qDebug() << "Protocol Error: questions received" << questions;
}

void ICMessageHandler::handle_answers(QVariant answers_v)
{
    if (answers_v.canConvert<QList<ICAnswer> >()) {
        answers = answers_v.value<QList<ICAnswer> >();
    } else qDebug() << "Application Error: answer list cast error";
}

void ICMessageHandler::handle_senderip(QVariant senderip)
{
    if (senderip.canConvert<QHostAddress>()) {
        clientAddress = senderip.value<QHostAddress>();
    } else qDebug() << "Application Error: sender ip cast error";
}
