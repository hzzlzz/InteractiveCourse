#include <QApplication>
#include <QDialog>
#include <QNetworkInterface>
#include <QTranslator>
#include <QFile>

#include "icstartdialog.h"
#include "icserver.h"
#include "icmainwindow.h"
#include "icconfig.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QTranslator translator;
    translator.load(CHINESE_CN);
    a.installTranslator(&translator);

    QFile File("stylesheet.qss");
    File.open(QFile::ReadOnly);
    QString StyleSheet = QLatin1String(File.readAll());
    a.setStyleSheet(StyleSheet);

    ICStartDialog *dialog = new ICStartDialog;
    dialog->show();
    
    return a.exec();
}
