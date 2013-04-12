#ifndef PARSER_H
#define PARSER_H

#include <QList>
#include "mapper.h"

class Handler;
class Source;
class QVariant;

class Parser
{
public:
    Parser(Handler *theHandler);
    void parse(Source* source, const QList<QVariant> paramList = QList<QVariant>());
protected:
    void addMapper(Mapper mapper);
private:
    QList<Mapper> mappers;
    Handler *handler;
};

#endif // PARSER_H
