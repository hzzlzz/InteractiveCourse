#include "icstartdialog.h"
#include <QPushButton>
#include <QTimer>
#include <QMessageBox>
#include <QNetworkInterface>
#include "icmainwindow.h"
#include "icconfig.h"

ICStartDialog::ICStartDialog(QWidget *parent) :
    QDialog(parent), isRunning(false)
{
    setupUi(this);
    setWindowIcon(QIcon(":/images/icimage.png"));

    okButton->setEnabled(false);

    server = new ICServer;

    QList<QNetworkInterface> if_list = QNetworkInterface::allInterfaces();

    foreach(const QNetworkInterface& interface, if_list) {
        QNetworkInterface::InterfaceFlags if_flags = interface.flags();

        // skip internal and down interfaces
        if(if_flags & QNetworkInterface::IsLoopBack
                || if_flags & QNetworkInterface::IsPointToPoint
                || !(if_flags & QNetworkInterface::IsUp)
                || !(if_flags & QNetworkInterface::IsRunning)) continue;

        // skip not configured... for sure...
        QList<QNetworkAddressEntry> if_ips = interface.addressEntries();
        foreach (const QNetworkAddressEntry nw_entry, if_ips) {
            if (nw_entry.ip().protocol() == QAbstractSocket::IPv4Protocol) {
                ipComboBox->addItem(interface.humanReadableName());
                addresses.append(nw_entry.ip());
                break;
            }
        }
    }

    if (addresses.size() == 0) {
        ipComboBox->setItemText(0, tr("No adapter found"));
        okButton->setEnabled(false);
    }

    QRegExp regExp("[A-z0-9]{3,10}");
    sidLineEdit->setValidator(new QRegExpValidator(regExp, this));

    connect(okButton, SIGNAL(clicked()), this, SLOT(testSid()));
    connect(cancelButton, SIGNAL(clicked()), this, SLOT(close()));
    connect(server, SIGNAL(identifierCollision(QString)), this, SLOT(collision()));

    connect(&timer, SIGNAL(timeout()), this, SLOT(sid_passed()));

    ifCollision = false;
}

void ICStartDialog::on_sidLineEdit_textChanged()
{
    okButton->setEnabled(sidLineEdit->hasAcceptableInput() && addresses.size() != 0);
}

void ICStartDialog::testSid()
{
    ifCollision = false;

    sidLineEdit->setEnabled(false);
    okButton->setEnabled(false);
    ipComboBox->setEnabled(false);

    statusLabel->setText(tr("checking uniqueness..."));

    timer.setInterval(SERVER_SID_TEST_DURATION);
    timer.setSingleShot(true);

    server->setUp(sidLineEdit->text(), addresses.value(ipComboBox->currentIndex()));

    if (!server->startSidTest(SERVER_SID_TEST_DURATION)) {
        statusLabel->setText("");
        sidLineEdit->setEnabled(true);
        okButton->setEnabled(true);

        QMessageBox::critical(this, tr("Error"),
                              QString(tr("Testing port %1 occupied.")).arg(QString::number(CLIENT_PORT)),
                              QMessageBox::Ok);
    }
    else {
        timer.start();
    }
}

void ICStartDialog::collision()
{
    if (!ifCollision) {
        ifCollision = true;
        timer.stop();
        statusLabel->setText("");
        sidLineEdit->setEnabled(true);
        okButton->setEnabled(true);
        ipComboBox->setEnabled(true);

        QMessageBox::warning(this, tr("Collision"),
                             tr("Server identifier collision.\nPlease change to another name."),
                             QMessageBox::Ok);
    }
}

void ICStartDialog::sid_passed()
{
    if (!server->listen()) {
        statusLabel->setText("");
        sidLineEdit->setEnabled(true);
        okButton->setEnabled(true);
        ipComboBox->setEnabled(true);

        QMessageBox::critical(this, tr("Error"),
                              QString(tr("Port %1 occupied.")).arg(QString::number(SERVER_PORT)),
                              QMessageBox::Ok);
    }
    else {
        ICMainWindow *window = new ICMainWindow(server);
        window->show();
        isRunning = true;

        close();
    }
}

/*
void ICStartDialog::cleanUp()
{
    disconnect(server, SIGNAL(identifierCollision(QString)), this, SLOT(collision()));
    if (!isRunning) {
        delete server;
    }
}

void ICStartDialog::closeEvent(QCloseEvent *)
{
    //cleanUp();
}
*/
