#ifndef HANDLER_H
#define HANDLER_H

#include <QObject>

class Handler : public QObject
{
    Q_OBJECT
public:
    explicit Handler(QObject *parent = 0);

    virtual void reset(QList<QVariant> paramList) = 0;
    void handle(QString suffix, QList<QVariant> valueList);
    virtual void done() = 0;
};

#endif // HANDLER_H
