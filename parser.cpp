#include "parser.h"

#include <QString>
#include "handler.h"
#include "source.h"

Parser::Parser(Handler *theHandler)
{
    handler = theHandler;
}

void Parser::addMapper(Mapper mapper)
{
    mappers.append(mapper);
}

void Parser::parse(Source* source, QList<QVariant> paramList)
{
    handler->reset(paramList);

    QListIterator<Mapper> i(mappers);
    while (i.hasNext()) {
        const Mapper mapper = i.next();
        QList<QString> keys = source->getKeys();
        QListIterator<QString> s(keys);
        while (s.hasNext()) {
            QString key = s.next();
            QList<QVariant> valueList = source->getValueList(key);
            mapper.action(handler, key, valueList);
        }
    }
    handler->done();
}
