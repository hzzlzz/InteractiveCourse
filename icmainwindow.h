#ifndef ICMAINWINDOW_H
#define ICMAINWINDOW_H

#include <QMainWindow>
#include <QPoint>

#include "ui_icmainwindow.h"
#include "icnewquestiondialog.h"
#include "icresultchart.h"

class ICServer;

class ICMainWindow : public QMainWindow, public Ui::ICMainWindow
{
    Q_OBJECT
public:
    explicit ICMainWindow(ICServer *theServer, QWidget *parent = 0);

    ~ICMainWindow();

private slots:
    void understand();
    void question();
    void stop();

    void newAnswerArrived(QString uid);
    void newQuestionReady(int type, int choices);

    void connectionClosed();

    QRegion getRegion() const;
    void updateMask();

private:

    void paintEvent(QPaintEvent* event);

    void mousePressEvent(QMouseEvent *e);
    void mouseMoveEvent(QMouseEvent *e);
    void mouseReleaseEvent(QMouseEvent *e);

    ICServer *server;
    ICNewQuestionDialog *questionDialog;

    bool onCanvas;
    QPoint lastPosition;

    QSet<QString> uids;
};

#endif // ICMAINWINDOW_H
