#include "icmessageparser.h"
#include "icmessagemapper.h"
#include "icmessagehandler.h"

ICMessageParser::ICMessageParser(ICMessageHandler *handler) : Parser(handler)
{
    ICMessageMapper mapper;
    addMapper(mapper);
}

ICMessageParser::~ICMessageParser() {}
