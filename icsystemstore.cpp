#include "icsystemstore.h"

#include <QMutexLocker>
#include <QMap>

ICSystemStore* ICSystemStore::instance = 0;
int ICSystemStore::questionCount = 0;

ICSystemStore * ICSystemStore::getInstance()
{
    static QMutex mutex;
    if (!instance) {
        mutex.lock();

        if (!instance) {
            instance = new ICSystemStore;
        }

        mutex.unlock();
    }

    return instance;
}

void ICSystemStore::destroy()
{
    static QMutex mutex;
    if (instance) {
        mutex.lock();
        if (instance) delete instance;
        instance = 0;
        mutex.unlock();
    }
}

ICQuestion& ICSystemStore::createQuestion()
{
    QMutexLocker locker(&objectMutex);
    QString id = QString::number(++questionCount);
    ICQuestion &question = allQuestions[id];
    question.setId(id);
    return question;
}

void ICSystemStore::setCurrentQuestion(ICQuestion& theQuestion)
{
    QMutexLocker locker(&objectMutex);
    currentQuestion = theQuestion;
    //currentAnswers.clear();
}

const ICQuestion ICSystemStore::getCurrentQuestion()
{
    QMutexLocker locker(&objectMutex);
    return currentQuestion;
}

/*
const int ICSystemStore::getCurrentAnswerSize()
{
    return currentAnswers.size();
}
*/

/*
const QList<ICAnswer> ICSystemStore::getCurrentAnswers()
{
    QMutexLocker locker(&objectMutex);

    return currentAnswers.values();
    /*
    QMap<QString, int> result;
    QHash<QString, ICAnswer>::const_iterator i = currentAnswers.constBegin();
    while (i != currentAnswers.constEnd()) {
        QString answer = i.value().getAnswer();
        int count = result.value(answer, 0);
        result.insert(answer, ++count);
    }
    return result;
}
*/

void ICSystemStore::addAnswer(const ICAnswer& answer)
{
    QMutexLocker locker(&objectMutex);
    if (!answer.isNull()) {
        allAnswers.insert(answer.getId(), answer);
    }
}

/*
void ICSystemStore::addCurrentAnswer(const ICAnswer& answer)
{
    QMutexLocker locker(&objectMutex);
    if (!answer.isNull()) {
        // we actually save all the answers
        allAnswers.insert(answer.getId(), answer);

        //if (!currentQuestion.isNull() && currentQuestion.getId().compare(answer.getId()) == 0) {
            // only save last submit for individual users, do not check
            currentAnswers.insert(answer.getUid(), answer);
        //}
    }
}
*/

const ICQuestion ICSystemStore::getQuestion(QString id)
{
    QMutexLocker locker(&objectMutex);
    return allQuestions.value(id);
}

const QList<ICQuestion> ICSystemStore::getAllQuestions()
{
    QMutexLocker locker(&objectMutex);
    return allQuestions.values();
}

const QList<ICAnswer> ICSystemStore::getAnswers(QString id)
{
    QMutexLocker locker(&objectMutex);
    return allAnswers.values(id);
}

