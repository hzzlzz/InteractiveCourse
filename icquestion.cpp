#include "icquestion.h"

#include <QJsonObject>
#include <QJsonValue>

const QString ICQuestion::idTag = "id";
const QString ICQuestion::typeTag = "type";
const QString ICQuestion::choicesTag = "choices";

ICQuestion::ICQuestion() {}

void ICQuestion::setId(QString questionId) { id = questionId; }
void ICQuestion::setType(QString questionType) { type = questionType; }
void ICQuestion::setChoices(QString theChoices) { choices = theChoices; }

QString ICQuestion::getId() const { return id; }
QString ICQuestion::getType() const { return type; }
QString ICQuestion::getChoices() const { return choices; }

bool ICQuestion::isNull() const { return id.isNull(); }

QJsonObject ICQuestion::saveAsJsonObject() const
{
    QJsonObject jo;
    jo.insert(idTag, QJsonValue(getId()));
    jo.insert(typeTag, QJsonValue(getType()));
    jo.insert(choicesTag, QJsonValue(getChoices()));

    return jo;
}
ICQuestion ICQuestion::readFromJsonObject(QJsonObject jsonObject)
{
    ICQuestion question;
    question.setId(jsonObject.value(idTag).toString());
    question.setType(jsonObject.value(typeTag).toString());
    question.setChoices(jsonObject.value(choicesTag).toString());

    return question;
}
