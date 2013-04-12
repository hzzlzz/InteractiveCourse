#include "icmessageparser.h"
#include "icmessagemapper.h"
#include "handler.h"

ICMessageParser::ICMessageParser(ICMessageHandler *handler) : Parser((Handler*) handler)
{
    ICMessageMapper mapper;
    addMapper(mapper);
}

ICMessageParser::~ICMessageParser() {}
