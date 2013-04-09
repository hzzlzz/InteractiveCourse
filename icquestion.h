#ifndef ICQUESTION_H
#define ICQUESTION_H

#include <QMetaType>
#include <QList>
#include <QString>

class QJsonObject;

class ICQuestion
{
public:
    ICQuestion();

    void setId(QString questionId);
    void setType(QString questionType);
    void setChoices(QString choices);

    QString getId() const;
    QString getType() const;
    QString getChoices() const;

    bool isNull() const;

    QJsonObject saveAsJsonObject() const;
    static ICQuestion readFromJsonObject(QJsonObject jsonObject);
private:
    static const QString idTag;
    static const QString typeTag;
    static const QString choicesTag;

    QString id;
    QString type;
    QString choices;
};

Q_DECLARE_METATYPE(ICQuestion)
#endif // ICQUESTION_H
