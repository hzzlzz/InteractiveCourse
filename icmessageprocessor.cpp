#include "icmessageprocessor.h"

#include "icmessageparser.h"
#include "icmessage.h"

ICMessageProcessor::ICMessageProcessor(ICMessageHandler *handler, QByteArray datagram, QObject *parent) :
    QObject(parent), parser(ICMessageParser(handler))
{
    message = ICMessage::readJsonFrom(datagram);
}

void ICMessageProcessor::run()
{
    parser.parse(&message);
}
