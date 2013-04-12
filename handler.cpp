#include "handler.h"
#include <QVariant>

Handler::Handler(QObject *parent) : QObject(parent) {}

void Handler::handle(const QString suffix, const QList<QVariant>& valueList)
{
    QString member_qs = "handle_" + suffix;
    std::string s = member_qs.toLatin1().constData();
    QMetaObject::invokeMethod(this, s.c_str(), Qt::DirectConnection,
                              Q_ARG(QVariant, valueList.value(0)));
}
