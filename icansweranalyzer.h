#ifndef ICANSWERANALYZER_H
#define ICANSWERANALYZER_H

#include <QHash>
#include <QMap>

#include "icquestion.h"
#include "icanswer.h"

class ICAnswerAnalyzer
{
public:
    ICAnswerAnalyzer(const ICQuestion& question);
    const QMap<QString, int> analyze(const QList<ICAnswer>& answers);
private:
    ICQuestion question;
    QHash<QString, QString> map;
};

#endif // ICANSWERANALYZER_H
