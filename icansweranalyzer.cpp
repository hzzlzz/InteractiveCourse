#include "icansweranalyzer.h"
#include "icconfig.h"
#include <QObject>
#include <QSet>

ICAnswerAnalyzer::ICAnswerAnalyzer(const ICQuestion& theQuestion) : question(theQuestion)
{
    int type = question.getType().toInt();
    switch (type) {
    case UNDERSTAND:
        map.insert("A", QObject::tr("A.Understand"));
        map.insert("B", QObject::tr("B.Nope"));
        map.insert("C", QObject::tr("C.Is it edible?"));
        break;
    case TOF:
        map.insert("A", QObject::tr("A.Yes"));
        map.insert("B", QObject::tr("B.No"));
        break;
    }
}

const QMap<QString, int> ICAnswerAnalyzer::analyze(const QList<ICAnswer>& answers)
{
    QMap<QString, int> result;
    QSet<QString> answeredUid;

    QListIterator<ICAnswer> i(answers);
    i.toBack();

    while (i.hasPrevious()) {
        const ICAnswer& answer = i.previous();
        QString uid = answer.getUid();
        if (!answeredUid.contains(uid)) {
            answeredUid.insert(uid);
            QString text = answer.getAnswer().trimmed();
            QString as;
            if (text.isEmpty()) {
                as = QObject::tr("Empty Answer");
            } else {
                as = map.value(text, text);
            }
            int count = result.value(as, 0);
            result.insert(as, ++count);
        }
    }
    return result;
}
