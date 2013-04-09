#ifndef ICRESULTWINDOW_H
#define ICRESULTWINDOW_H

#include <QMainWindow>
#include "icresultchart.h"

class QComboBox;
class QToolButton;

class ICResultWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit ICResultWindow(ICResultChart *chart ,QWidget *parent = 0);
    ~ICResultWindow();

private:
    ICResultChart *d_chart;
    QComboBox *orientationBox;
    QToolButton *btnExport;
};

#endif // ICRESULTWINDOW_H
