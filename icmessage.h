#ifndef ICMESSAGE_H
#define ICMESSAGE_H

#include <QHash>
#include <QHostAddress>
#include "source.h"
#include "icquestion.h"
#include "icanswer.h"

class ICMessage : public Source
{
public:
    ICMessage();
    QList<QString> getKeys() const;
    QList<QVariant> getValueList(QString key) const;

    void setProperty(QString key, QString value);
    void setQuestions(QList<ICQuestion> theQuestions);
    void setAnswers(QList<ICAnswer> theAnswers);
    void setSender(QHostAddress theSender);

    QString getProperty(QString key) const;
    QList<ICQuestion> getQuestions() const;
    QList<ICAnswer> getAnswers() const;
    QHostAddress getSender() const;

    QByteArray saveAsJson() const;
    static ICMessage readJsonFrom(QByteArray datagram);

    static ICMessage initWithCommonProperties();

private:
    QHash<QString, QString> properties;
    QList<ICQuestion> questions;
    QList<ICAnswer> answers;
    QHostAddress sender;
};

#endif // ICMESSAGE_H
