#include "handler.h"
#include <QVariant>

Handler::Handler(QObject *parent) : QObject(parent) {}

void Handler::handle(QString suffix, QList<QVariant> valueList)
{
    QString member_qs = "handle_" + suffix;
    std::string s = member_qs.toLatin1().constData();
    QMetaObject::invokeMethod(this, s.c_str(), Qt::QueuedConnection,
                              Q_ARG(QVariant, valueList.at(0)),
                              Q_ARG(QVariant, valueList.at(1)),
                              Q_ARG(QVariant, valueList.at(2)),
                              Q_ARG(QVariant, valueList.at(3)),
                              Q_ARG(QVariant, valueList.at(4)),
                              Q_ARG(QVariant, valueList.at(5)),
                              Q_ARG(QVariant, valueList.at(6)),
                              Q_ARG(QVariant, valueList.at(7)),
                              Q_ARG(QVariant, valueList.at(8)),
                              Q_ARG(QVariant, valueList.at(9)));
}
