#include "icstartdialog.h"
#include <QPushButton>
#include <QTimer>
#include <QMessageBox>
#include "icmainwindow.h"
#include "icconfig.h"

ICStartDialog::ICStartDialog(ICServer *theServer, QWidget *parent) :
    QDialog(parent), server(theServer)
{
    setupUi(this);
    buttonBox->button(QDialogButtonBox::Ok)->setEnabled(false);

    QRegExp regExp("[A-z0-9]{3,10}");
    sidLineEdit->setValidator(new QRegExpValidator(regExp, this));

    connect(buttonBox, SIGNAL(accepted()), this, SLOT(test_sid()));
    connect(buttonBox, SIGNAL(rejected()), this, SLOT(reject()));
    connect(server, SIGNAL(identifierCollision(QString)), this, SLOT(collision()));

    connect(&timer, SIGNAL(timeout()), this, SLOT(sid_passed()));
}

void ICStartDialog::on_sidLineEdit_textChanged()
{
    buttonBox->button(QDialogButtonBox::Ok)->setEnabled(sidLineEdit->hasAcceptableInput());
    // this place can test conflict
}

void ICStartDialog::test_sid()
{
    sidLineEdit->setEnabled(false);
    statusLabel->setText("checking uniqueness...");
    buttonBox->button(QDialogButtonBox::Ok)->setEnabled(false);

    timer.setInterval(SERVER_SID_TEST_DURATION);
    timer.setSingleShot(true);

    server->setIdentifier(sidLineEdit->text());
    if (!server->startTest(SERVER_SID_TEST_DURATION)) {
        statusLabel->setText("");
        sidLineEdit->setEnabled(true);
        QMessageBox::critical(this, QString(tr("Testing Port %1 occupied")).arg(QString::number(CLIENT_PORT)),
                             tr("Server startup failed."),
                             QMessageBox::Ok);
    }
    else {
        timer.start();
    }
}

void ICStartDialog::collision()
{
    timer.stop();

    QMessageBox::warning(this, tr("Collision"),
                         tr("Server identifier collision.\n"
                            "Please change to another name."),
                         QMessageBox::Ok);

    sidLineEdit->setEnabled(true);
}

void ICStartDialog::sid_passed()
{
    if (!server->listen()) {
        statusLabel->setText("");
        sidLineEdit->setEnabled(true);

        QMessageBox::critical(this, QString(tr("Port %1 occupied")).arg(QString::number(SERVER_PORT)),
                             tr("Server startup failed."),
                             QMessageBox::Ok);
    }
    else {
        ICMainWindow *window = new ICMainWindow(server);
        window->show();

        close();
    }
}

ICStartDialog::~ICStartDialog()
{
     disconnect(server, SIGNAL(identifierCollision(QString)), this, SLOT(collision()));
}
