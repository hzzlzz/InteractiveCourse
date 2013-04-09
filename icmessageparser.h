#ifndef ICMESSAGEPARSER_H
#define ICMESSAGEPARSER_H

#include "parser.h"

class ICMessageMapper;
class ICMessageHandler;

class ICMessageParser : public Parser
{
public:
    ICMessageParser(ICMessageHandler* handler);
    ~ICMessageParser();
};

#endif // ICMESSAGEPARSER_H
