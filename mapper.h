#ifndef MAPPER_H
#define MAPPER_H

#include <QString>
#include <QHash>
#include <QVariant>

class Handler;

class Mapper
{
public:
    Mapper();
    void action(Handler* handler, QString key, QList<QVariant>& valueList) const;
protected:
    void addMapping(QString key, QString suffix);
    QString getSuffix(QString key) const;
private:
    QHash<QString, QString> mappings;
};

#endif // MAPPER_H
