#ifndef ICMESSAGEPROCESSOR_H
#define ICMESSAGEPROCESSOR_H

#include <QObject>
#include <QRunnable>
#include <QByteArray>

#include "icmessage.h"
#include "icmessageparser.h"

class ICMessageHandler;

class ICMessageProcessor : public QObject, public QRunnable
{
    Q_OBJECT
public:
    explicit ICMessageProcessor(ICMessageHandler *handler, QByteArray datagram, QObject *parent = 0);
    
    void run();
signals:
    
public slots:
    
private:
    ICMessageHandler *handler;
    ICMessageParser parser;
    ICMessage message;
};

#endif // ICMESSAGEPROCESSOR_H
