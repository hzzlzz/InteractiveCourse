#ifndef ICSYSTEMSTORE_H
#define ICSYSTEMSTORE_H

#include <QList>
#include <QHash>
#include <QMutex>

#include "icquestion.h"
#include "icanswer.h"

class QString;

class ICSystemStore
{
public:
    static ICSystemStore * getInstance();
    // prevent memory leakage, require manul invocation if life of this store object is permanent
    static void destroy();

    // handle id issue
    ICQuestion& createQuestion();

    // set or clear current question
    void setCurrentQuestion(ICQuestion& theQuestion);

    const ICQuestion getCurrentQuestion();
    //const int getCurrentAnswerSize();
    //const QList<ICAnswer> getCurrentAnswers();

    // simply add an answer
    void addAnswer(const ICAnswer& answer);

    // also add answer to current answers, one uid one answer
    //void addCurrentAnswer(const ICAnswer& answer);

    const ICQuestion getQuestion(QString id);

    const QList<ICQuestion> getAllQuestions();
    const QList<ICAnswer> getAnswers(QString id);

private:
    ICSystemStore() {}

    ICSystemStore(const ICSystemStore &);
    ICSystemStore& operator=(const ICSystemStore &);

    static ICSystemStore *instance;
    static int questionCount;

    QHash<QString, ICQuestion> allQuestions;
    QMultiHash<QString, ICAnswer> allAnswers;

    ICQuestion currentQuestion;
    //QHash<QString, ICAnswer> currentAnswers;

    QMutex objectMutex;
};

#endif // ICSYSTEMSTORE_H
