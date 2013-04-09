#include <QApplication>
#include <QDialog>

#include "icstartdialog.h"
#include "icserver.h"
#include "icmainwindow.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    ICServer *server = new ICServer;
    ICStartDialog *dialog = new ICStartDialog(server);
    dialog->show();
    
    return a.exec();
}
