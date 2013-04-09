#ifndef SOURCE_H
#define SOURCE_H

#include <QString>
#include <QList>
#include <QVariant>

class Source
{
public:
    virtual QList<QString> getKeys() const = 0;
    virtual QList<QVariant> getValueList(QString key) const = 0;
};

#endif // SOURCE_H
