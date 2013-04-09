#include "icmainwindow.h"

#include <QMouseEvent>
#include <QMessageBox>

#include "icsystemstore.h"
#include "icquestion.h"
#include "icserver.h"
#include "icresultwindow.h"
#include "icresultchart.h"

#include "icconfig.h"

ICMainWindow::ICMainWindow(ICServer *theServer, QWidget *parent) :
    QMainWindow(parent), server(theServer), questionDialog(new ICNewQuestionDialog(this)),
    onCanvas(false)
{
    setupUi(this);

    //setAttribute(Qt::WA_TranslucentBackground);
    //setStyleSheet("background:transparent;");
    setWindowFlags(Qt::FramelessWindowHint);
    //setStyleSheet("background:transparent;");
    //questionDialog->setWindowOpacity(0.8);
    //questionDialog->setWindowFlags(Qt::FramelessWindowHint);

    connect(exitButton, SIGNAL(clicked()), this, SLOT(close()));
    connect(understandButton, SIGNAL(clicked()), this, SLOT(understand()));
    connect(questionButton, SIGNAL(clicked()), this, SLOT(question()));
    connect(stopButton, SIGNAL(clicked()), this, SLOT(stop()));

    connect(server, SIGNAL(newAnswerArrived(QString)), this, SLOT(newAnswerArrived(QString)));
    connect(server, SIGNAL(connectionClosed()), this, SLOT(connectionClosed()));

    connect(questionDialog, SIGNAL(newQuestion(int,int)), this, SLOT(newQuestionReady(int,int)));

    countLabel->setText("0/0");
}

void ICMainWindow::understand()
{
    understandButton->setEnabled(false);
    questionButton->setEnabled(false);

    ICSystemStore *store = ICSystemStore::getInstance();

    ICQuestion& us_question = store->createQuestion();
    us_question.setType(QString(UNDERSTAND));
    store->setCurrentQuestion(us_question);
    uids.clear();

    server->broadcastQuestion();

    countLabel->setText(us_question.getId() + "/" + QString::number(uids.size()));

    stopButton->setEnabled(true);
}

void ICMainWindow::question()
{
    questionDialog->move(this->pos().x(),this->pos().y()+this->size().height());
    questionDialog->exec();
}

void ICMainWindow::stop()
{
    stopButton->setEnabled(false);

    ICSystemStore *store = ICSystemStore::getInstance();
    QString id = store->getCurrentQuestion().getId();
    QList<ICAnswer> answers = store->getAnswers(id);

    QMap<QString, int> result;
    QSet<QString> answeredUid;

    QListIterator<ICAnswer> i(answers);
    i.toBack();
    while (i.hasPrevious()) {
        const ICAnswer& answer = i.previous();
        QString uid = answer.getUid();
        if (!answeredUid.contains(uid)) {
            answeredUid.insert(uid);
            QString text = answer.getAnswer();
            int count = result.value(text, 0);
            result.insert(text, ++count);
        }
    }
    ICResultChart *resultChart = new ICResultChart(this);
    resultChart->setTitle(QString(tr("Result of question %1")).arg(id));
    resultChart->setResult(result);
    resultChart->populate();
    ICResultWindow *resultWindow = new ICResultWindow(resultChart, this);
    resultWindow->resize(600, 400);
    resultWindow->show();

    store->setCurrentQuestion(ICQuestion());

    understandButton->setEnabled(true);
    questionButton->setEnabled(true);
}

void ICMainWindow::newAnswerArrived(QString uid)
{
    uids.insert(uid);
    countLabel->setText(ICSystemStore::getInstance()->getCurrentQuestion().getId() + "/" + QString::number(uids.size()));
}

void ICMainWindow::newQuestionReady(int type, int choices)
{
    understandButton->setEnabled(false);
    questionButton->setEnabled(false);

    ICSystemStore *store = ICSystemStore::getInstance();

    ICQuestion& question = store->createQuestion();
    question.setType(QString(type));
    if (type != TOF) question.setChoices(QString(choices));
    store->setCurrentQuestion(question);
    uids.clear();

    server->broadcastQuestion();

    countLabel->setText(question.getId() + "/" + uids.size());

    stopButton->setEnabled(true);
}

void ICMainWindow::connectionClosed()
{
    understandButton->setEnabled(false);
    questionButton->setEnabled(false);
    stopButton->setEnabled(false);

    QMessageBox::critical(this, tr("Error"),
                         tr("Connection failed."),
                         QMessageBox::Ok);
}

void ICMainWindow::mousePressEvent(QMouseEvent *e)
{
    onCanvas = true;
    lastPosition = e->globalPos();
}

void ICMainWindow::mouseMoveEvent(QMouseEvent *e)
{
    if (onCanvas == true) {
        int dx = e->globalX() - lastPosition.x();
        int dy = e->globalY() - lastPosition.y();

        lastPosition = e->globalPos();

        this->move(x()+dx, y()+dy);
    }
}

void ICMainWindow::mouseReleaseEvent(QMouseEvent *e)
{
    if (onCanvas == true) onCanvas = false;
}

ICMainWindow::~ICMainWindow()
{
    delete questionDialog;
}
