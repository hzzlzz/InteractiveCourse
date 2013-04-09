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
    explicit ICStartDialog(ICServer *server, QWidget *parent = 0);
    ~ICStartDialog();
signals:
    
public slots:

private slots:
    void on_sidLineEdit_textChanged();

    void test_sid();
    void sid_passed();
    void collision();
private:
    ICServer *server;
    QTimer timer;
};

#endif // ICSTARTDIALOG_H
