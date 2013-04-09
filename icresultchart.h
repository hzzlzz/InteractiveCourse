#ifndef ICRESULTCHART_H
#define ICRESULTCHART_H

#include <qwt_plot.h>
#include <QMap>
#include <QString>

class QwtPlotBarChart;

class ICResultChart : public QwtPlot
{
    Q_OBJECT
public:
    explicit ICResultChart(QWidget *parent = 0);
    void setChartTitle(QString title);
    void setResult(const QMap<QString, int>&);
    void populate();

public slots:
    void setOrientation( int );
    void exportChart();

private:
    QwtPlotBarChart *d_barChartItem;
    QMap<QString, int> result;
};

#endif // ICRESULTCHART_H
