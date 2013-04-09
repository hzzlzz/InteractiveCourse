#include "icanswer.h"

#include <QJsonObject>
#include <QJsonValue>

const QString ICAnswer::idTag = "id";
const QString ICAnswer::textTag = "text";

ICAnswer::ICAnswer() {}

void ICAnswer::setId(QString questionId) { id = questionId; }
void ICAnswer::setAnswer(QString theAnswer) { answer = theAnswer; }

QString ICAnswer::getId() const { return id; }
QString ICAnswer::getAnswer() const { return answer; }

void ICAnswer::setUid(QString theUid) { uid = theUid; }
QString ICAnswer::getUid() const { return uid; }

bool ICAnswer::isNull() const { return id.isNull(); }

QJsonObject ICAnswer::saveAsJsonObject() const
{
    QJsonObject jo;
    jo.insert(idTag, QJsonValue(getId()));
    jo.insert(textTag, QJsonValue(getAnswer()));
    return jo;
}
ICAnswer ICAnswer::readFromJsonObject(QJsonObject jsonObject)
{
    ICAnswer answer;
    answer.setId(jsonObject.value(idTag).toString());
    answer.setAnswer(jsonObject.value(textTag).toString());
    return answer;
}
