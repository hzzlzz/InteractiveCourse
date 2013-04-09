#include "icresultchart.h"
#include <qwt_plot_barchart.h>
#include <qwt_plot_renderer.h>
#include <qwt_column_symbol.h>
#include <qwt_plot_layout.h>
#include <qwt_legend.h>
#include <qwt_scale_draw.h>

class ChoicesScaleDraw: public QwtScaleDraw
{
public:
    ChoicesScaleDraw(const QList<QString>& base) :
        labels(base) {}
    virtual QwtText lable(double v) const
    {
        return labels.at(static_cast<int>(v));
    }
private:
    QList<QString> labels;
};

ICResultChart::ICResultChart(QWidget *parent) :
    QwtPlot(parent)
{

    setAutoFillBackground(true);

    setPalette(Qt::white);
    canvas()->setPalette(QColor("LemonChiffon"));

    setAxisTitle(QwtPlot::yLeft, tr("Number"));
    setAxisTitle(QwtPlot::xBottom, tr("Choices"));

    d_barChartItem = new QwtPlotBarChart("Quantity");
    d_barChartItem->setLayoutPolicy(QwtPlotBarChart::AutoAdjustSamples);
    d_barChartItem->setSpacing(20);
    d_barChartItem->setMargin(3);

    d_barChartItem->attach(this);

    insertLegend(new QwtLegend());

    setOrientation(0);

    setAutoReplot(true);
}

void ICResultChart::setChartTitle(QString title)
{
    setTitle(title);
}

void ICResultChart::setResult(const QMap<QString, int>& data)
{
    result = data;
}

void ICResultChart::populate()
{
    static const char *color = "SteelBlue";

    QwtColumnSymbol *symbol = new QwtColumnSymbol(QwtColumnSymbol::Box);
    symbol->setLineWidth(2);
    symbol->setFrameStyle(QwtColumnSymbol::Raised);
    symbol->setPalette(QPalette(color));

    d_barChartItem->setSymbol(symbol);

    QVector<double> samples;
    QMap<QString, int>::const_iterator i = result.begin();
    while (i != result.end()) {
        samples.append(i.value());
    }

    d_barChartItem->setSamples(samples);
}

void ICResultChart::setOrientation(int orientation)
{
    QwtPlot::Axis axis1, axis2;

    if (orientation == 0) {
        axis1 = QwtPlot::xBottom;
        axis2 = QwtPlot::yLeft;

        d_barChartItem->setOrientation(Qt::Vertical);
    } else {
        axis1 = QwtPlot::yLeft;
        axis2 = QwtPlot::xBottom;

        d_barChartItem->setOrientation(Qt::Horizontal);
    }

    setAxisScale(axis1, 0, d_barChartItem->dataSize(), 1.0);
    setAxisAutoScale(axis2);

    setAxisScaleDraw(axis1, new ChoicesScaleDraw(result.keys()));

    QwtScaleDraw *scaleDraw2 = axisScaleDraw(axis2);
    scaleDraw2->enableComponent(QwtScaleDraw::Backbone, true);
    scaleDraw2->enableComponent(QwtScaleDraw::Ticks, true);

    plotLayout()->setAlignCanvasToScale(axis1, true);
    plotLayout()->setAlignCanvasToScale(axis2, false);

    plotLayout()->setCanvasMargin(0);
    updateCanvasMargins();

    replot();
}

void ICResultChart::exportChart()
{
    QwtPlotRenderer renderer;
    renderer.exportTo(this, "icresult.pdf");
}
