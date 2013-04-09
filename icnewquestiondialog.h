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
};

#endif // ICNEWQUESTIONDIALOG_H
