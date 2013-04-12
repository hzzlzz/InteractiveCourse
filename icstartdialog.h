#ifndef ICSTARTDIALOG_H
#define ICSTARTDIALOG_H

#include "ui_icstartdialog.h"
#include "icserver.h"

#include <QDialog>
#include <QTimer>

class ICStartDialog : public QDialog, public Ui::ICStartDialog
{
    Q_OBJECT
public:
    explicit ICStartDialog(QWidget *parent = 0);
signals:
    
public slots:

private slots:
    void on_sidLineEdit_textChanged();

    void testSid();
    void sid_passed();
    void collision();

private:
    bool ifCollision;
    bool isRunning;
    ICServer *server;
    QTimer timer;
    QList<QHostAddress> addresses;
};

#endif // ICSTARTDIALOG_H
