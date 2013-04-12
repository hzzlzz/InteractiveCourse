#include "icmessage.h"

#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QHash>
#include <QDebug>
#include <QSettings>

#include <QMetaType>
Q_DECLARE_METATYPE(QHostAddress)

#include "icconfig.h"

ICMessage::ICMessage() {}

void ICMessage::setProperty(QString key, QString value) { properties.insert(key, value); }
void ICMessage::setQuestions(QList<ICQuestion> theQuestions) { questions = theQuestions; }
void ICMessage::setAnswers(QList<ICAnswer> theAnswers) { answers = theAnswers; }
void ICMessage::setSender(QHostAddress theSender) { sender = theSender; }

QString ICMessage::getProperty(QString key) const { return properties.value(key); }
QList<ICQuestion> ICMessage::getQuestions() const { return questions; }
QList<ICAnswer> ICMessage::getAnswers() const { return answers; }
QHostAddress ICMessage::getSender() const { return sender; }

QList<QString> ICMessage::getKeys() const
{
    QList<QString> keys;
    keys.append(properties.keys());
    if (!questions.isEmpty()) keys.append(DEFAULT_QUESTIONS_TAG);
    if (!answers.isEmpty()) keys.append(DEFAULT_ANSWERS_TAG);
    if (!sender.isNull()) keys.append(SENDER_KEY);
    return keys;
}

QList<QVariant> ICMessage::getValueList(QString key) const
{
    QList<QVariant> valueList;

    // For normal property, return QList<QString> size 1
    QString value = properties.value(key);
    if (!value.isNull()) {
        valueList.append(value);
        return valueList;
    }
    // For questions (or answers), return QList<ICQuestion> (or QList<ICAnswer>)
    if (key.compare(DEFAULT_QUESTIONS_TAG) == 0) {
        QList<ICQuestion> questionList;
        foreach (const ICQuestion& question, questions) {
            questionList.append(question);
        }
        valueList.append(QVariant::fromValue<QList<ICQuestion> >(questionList));
        return valueList;
    }
    if (key.compare(DEFAULT_ANSWERS_TAG) == 0) {
        QList<ICAnswer> answerList;
        foreach (const ICAnswer& answer, answers) {
            answerList.append(answer);
        }
        valueList.append(QVariant::fromValue<QList<ICAnswer> >(answerList));
        return valueList;
    }
    // For sender, return QList<QHostAddress> size 1
    if (key.compare(SENDER_KEY) == 0) {
        valueList.append(QVariant::fromValue(sender));
        return valueList;
    }

    // No such key
    qDebug() << valueList;
    return valueList;
}

QByteArray ICMessage::saveAsJson() const
{
    QJsonObject jo;
    QHash<QString, QString>::const_iterator i = properties.begin();
    while(i != properties.end()) {
        jo.insert(i.key(), i.value());
        ++i;
    }
    if (!questions.isEmpty()) {
        QJsonArray ja;
        foreach (const ICQuestion& question, getQuestions()) {
            ja.append(QJsonValue(question.saveAsJsonObject()));
        }
        jo.insert(DEFAULT_QUESTIONS_TAG, ja);
    }
    if (!answers.isEmpty()) {
        QJsonArray ja;
        foreach (const ICAnswer& answer, getAnswers()) {
            ja.append(QJsonValue(answer.saveAsJsonObject()));
        }
        jo.insert(DEFAULT_ANSWERS_TAG, ja);
    }
    return QJsonDocument(jo).toJson();
}

ICMessage ICMessage::readJsonFrom(QByteArray datagram)
{
    QJsonDocument jd = QJsonDocument::fromJson(datagram);
    if(jd.isNull()) qDebug() << "incoming json is null";
    QJsonObject jo = jd.object();

    ICMessage message;
    QJsonObject::const_iterator i = jo.constBegin();
    while (i != jo.constEnd()) {
        QString key = i.key();
        QJsonValue value = i.value();
        if (value.isString()) {
            message.setProperty(key, value.toString());
        }
        if (value.isArray()) {
            if (key.compare(DEFAULT_QUESTIONS_TAG) == 0) {
                QJsonArray ja = value.toArray();
                QList<ICQuestion> questions;
                foreach (const QJsonValue& value, ja) {
                    if (value.isObject()) {
                        QJsonObject json_question = value.toObject();
                        questions.append(ICQuestion::readFromJsonObject(json_question));
                    }
                }
                message.setQuestions(questions);
            }
            else if (key.compare(DEFAULT_ANSWERS_TAG) == 0) {
                QJsonArray ja = value.toArray();
                QList<ICAnswer> answers;
                foreach (const QJsonValue& value, ja) {
                    if (value.isObject()) {
                        QJsonObject json_answer = value.toObject();
                        answers.append(ICAnswer::readFromJsonObject(json_answer));
                    }
                }
                message.setAnswers(answers);
            }
        }
        ++i;
    }
    return message;
}

ICMessage ICMessage::initWithCommonProperties()
{
    QSettings settings(CONFIG_FILE, QSettings::IniFormat);

    ICMessage message;
    message.setProperty(settings.value(APP_CONFIG_TAG,DEFAULT_APP_TAG).toString(),
                           settings.value(APP_CONFIG_VALUE,DEFAULT_APP_VALUE).toString());
    message.setProperty(settings.value(VERSION_CONFIG_TAG,DEFAULT_VERSION_TAG).toString(),
                           settings.value(VERSION_CONFIG_VALUE,DEFAULT_VERSION_VALUE).toString());
    return message;
}
