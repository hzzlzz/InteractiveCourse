#include "mapper.h"
#include "handler.h"

Mapper::Mapper()
{
}

void Mapper::addMapping(QString key, QString suffix)
{
    mappings.insert(key, suffix);
}

QString Mapper::getSuffix(QString key) const
{
    return mappings.value(key, NULL);
}

void Mapper::action(Handler *handler, QString key, QList<QVariant>& valueList) const
{
    QString suffix = getSuffix(key);
    if (suffix != NULL) handler->handle(suffix, valueList);
}
