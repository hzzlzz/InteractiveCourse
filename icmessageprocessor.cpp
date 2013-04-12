#include "icmessageprocessor.h"

#include "icmessageparser.h"
#include "icmessage.h"

#include <QMetaType>
Q_DECLARE_METATYPE(QHostAddress)

ICMessageProcessor::ICMessageProcessor(ICMessageHandler *handler, const QHostAddress& address, QByteArray datagram) :
    clientAddress(address)
{
    parser = new ICMessageParser(handler);
    message = ICMessage::readJsonFrom(datagram);
}

void ICMessageProcessor::run()
{
    QList<QVariant> list;
    list.append(QVariant::fromValue(clientAddress));
    parser->parse(&message, list);
}

ICMessageProcessor::~ICMessageProcessor()
{
    delete parser;
}
