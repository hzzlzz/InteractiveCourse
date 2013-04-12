#ifndef ICNEWQUESTIONDIALOG_H
#define ICNEWQUESTIONDIALOG_H

#include <QDialog>

#include "ui_icnewquestiondialog.h"

class ICNewQuestionDialog : public QDialog, public Ui::ICNewQuestionDialog
{
    Q_OBJECT
public:
    explicit ICNewQuestionDialog(QWidget *parent = 0);

signals:
    void newQuestion(int type, int choices);
private slots:
    void startQuestioning();
private:

    QRegion getRegion() const;
    void updateMask();

    void resizeEvent(QResizeEvent*);
    void paintEvent(QPaintEvent* event);

    void mousePressEvent(QMouseEvent *e);
    void mouseMoveEvent(QMouseEvent *e);
    void mouseReleaseEvent(QMouseEvent *e);

    QPoint lastPosition;
    bool onCanvas;
};

#endif // ICNEWQUESTIONDIALOG_H
