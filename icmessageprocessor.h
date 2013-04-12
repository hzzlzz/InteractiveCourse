#ifndef ICMESSAGEPROCESSOR_H
#define ICMESSAGEPROCESSOR_H

#include <QObject>
#include <QRunnable>
#include <QByteArray>

#include "icmessage.h"

class ICMessageHandler;
class ICMessageParser;

class ICMessageProcessor : public QRunnable
{
public:
    explicit ICMessageProcessor(ICMessageHandler *handler, const QHostAddress& address, QByteArray datagram);
    void run();

    ~ICMessageProcessor();
private:
    ICMessageHandler *handler;
    ICMessageParser *parser;
    QHostAddress clientAddress;
    ICMessage message;
};

#endif // ICMESSAGEPROCESSOR_H
