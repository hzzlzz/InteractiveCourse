#ifndef ICANSWER_H
#define ICANSWER_H

#include <QMetaType>
#include <QList>
#include <QString>

class QJsonObject;

class ICAnswer
{
public:
    ICAnswer();

    void setId(QString questionId);
    void setAnswer(QString answer);

    // uid not saved in json
    void setUid(QString uid);
    QString getUid() const;

    QString getId() const;
    QString getAnswer() const;

    bool isNull() const;

    QJsonObject saveAsJsonObject() const;
    static ICAnswer readFromJsonObject(QJsonObject jsonObject);

private:
    static const QString idTag;
    static const QString textTag;

    QString id;
    QString answer;

    // not saved;
    QString uid;
};

Q_DECLARE_METATYPE(ICAnswer)
#endif // ICANSWER_H
