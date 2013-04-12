#include "icmainwindow.h"

#include <QMouseEvent>
#include <QMessageBox>
#include <QBitmap>

#include "icsystemstore.h"
#include "icquestion.h"
#include "icserver.h"
#include "icresultwindow.h"
#include "icresultchart.h"
#include "icansweranalyzer.h"

#include "icconfig.h"

ICMainWindow::ICMainWindow(ICServer *theServer, QWidget *parent) :
    QMainWindow(parent), server(theServer), questionDialog(new ICNewQuestionDialog(this)),
    onCanvas(false)
{
    setupUi(this);

    setWindowIcon(QIcon(":/images/icimage.png"));

    //this->setAttribute(Qt::WA_TranslucentBackground, true);
    setWindowFlags(Qt::FramelessWindowHint);
    //questionDialog->setWindowOpacity(0.8);
    questionDialog->setWindowFlags(questionDialog->windowFlags() | Qt::FramelessWindowHint);

    connect(exitButton, SIGNAL(clicked()), this, SLOT(close()));
    connect(understandButton, SIGNAL(clicked()), this, SLOT(understand()));
    connect(questionButton, SIGNAL(clicked()), this, SLOT(question()));
    connect(stopButton, SIGNAL(clicked()), this, SLOT(stop()));

    connect(server, SIGNAL(newAnswerArrived(QString)), this, SLOT(newAnswerArrived(QString)));
    connect(server, SIGNAL(connectionClosed()), this, SLOT(connectionClosed()));

    connect(questionDialog, SIGNAL(newQuestion(int,int)), this, SLOT(newQuestionReady(int,int)));

    sidLabel->setText(server->getIdentifier());
    countLabel->setText("0/0");
}

QRegion ICMainWindow::getRegion() const
{
        QPainterPath path;
        path.addRoundedRect(rect().adjusted(1, -1, -1, 0), 6, 6);
        return QRegion(path.toFillPolygon().toPolygon());
}

void ICMainWindow::updateMask()
{
    QBitmap Bitmap(size());
    Bitmap.clear();
    QPainter Painter(&Bitmap);

    Painter.setClipRegion(getRegion());
    Painter.fillRect(rect(), Qt::color1);
    Painter.end();

    setMask(Bitmap);
}

void ICMainWindow::paintEvent(QPaintEvent* event)
{
    Q_UNUSED(event);

    updateMask();

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setPen(QPen(QBrush(QColor(51, 153, 255)), 2.0));
    painter.setBrush(QBrush(QColor(252, 252, 252)));
    painter.drawRoundedRect(rect().adjusted(2,1,-2,-1), 5, 5);
}

void ICMainWindow::understand()
{
    understandButton->setEnabled(false);
    questionButton->setEnabled(false);

    ICSystemStore *store = ICSystemStore::getInstance();

    ICQuestion& us_question = store->createQuestion();
    us_question.setType(QString::number(UNDERSTAND));
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
    ICQuestion currentQuestion = store->getCurrentQuestion();
    QString id = currentQuestion.getId();
    QList<ICAnswer> answers = store->getAnswers(id);

    ICAnswerAnalyzer analyzer(currentQuestion);
    QMap<QString, int> result = analyzer.analyze(answers);

    ICResultChart *resultChart = new ICResultChart(this);
    resultChart->setTitle(QString(tr("Result of Question %1")).arg(id));
    resultChart->setResult(result);
    resultChart->populate();

    ICResultWindow *resultWindow = new ICResultWindow(resultChart, this);
    resultWindow->resize(800, 600);
    resultWindow->show();

    ICQuestion question;
    store->setCurrentQuestion(question);

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
    question.setType(QString::number(type));
    if (type != TOF) question.setChoices(QString::number(choices));
    store->setCurrentQuestion(question);
    uids.clear();

    server->broadcastQuestion();

    countLabel->setText(question.getId() + "/" + QString::number(uids.size()));

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
    delete server;
    ICSystemStore::destroy();
}
