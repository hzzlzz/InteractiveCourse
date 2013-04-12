#include "icresultchart.h"
#include "icconfig.h"
#include <qwt_plot_barchart.h>
#include <qwt_plot_renderer.h>
#include <qwt_column_symbol.h>
#include <qwt_plot_layout.h>
#include <qwt_plot_grid.h>
#include <qwt_legend.h>
#include <qwt_scale_draw.h>
#include <qwt_plot_abstract_barchart.h>

template <typename T>
QList<T> reversed(const QList<T> & in) {
    QList<T> result;
    result.reserve(in.size());
    std::reverse_copy(in.begin(), in.end(), std::back_inserter(result));
    return result;
}

class ChoicesScaleDraw: public QwtScaleDraw
{
public:
    ChoicesScaleDraw(const QList<QString>& base) :
        labels(base) {}
    virtual QwtText label(double v) const
    {
        double i;
        if (modf(abs(v), &i) < 0.00000001) {
            return labels.value(static_cast<int>(v));
        }
        return "";
    }
private:
    QList<QString> labels;
};

ICResultChart::ICResultChart(QWidget *parent) :
    QwtPlot(parent)
{

    setAutoFillBackground(true);

    setPalette(Qt::white);
    canvas()->setPalette(Qt::gray);

    d_barChartItem = new QwtPlotBarChart("Quantity");
    //d_barChartItem->setLayoutPolicy(QwtPlotBarChart::AutoAdjustSamples);
    //d_barChartItem->setSpacing(MIN_SPACING);
    //d_barChartItem->setMargin(MIN_MARGIN);

    d_barChartItem->attach(this);

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
    QwtPlotGrid *grid = new QwtPlotGrid;
    grid->enableX( false );
    grid->enableY( true );
    grid->enableXMin( false );
    grid->enableYMin( false );
    grid->setMajorPen( Qt::black, 0, Qt::DotLine );
    grid->attach( this );

    QColor color = Qt::blue;
    color.setAlpha(180);

    QwtColumnSymbol *symbol = new QwtColumnSymbol(QwtColumnSymbol::Box);
    symbol->setLineWidth(2);
    symbol->setFrameStyle(QwtColumnSymbol::Raised);
    symbol->setPalette(QPalette(color));

    d_barChartItem->setSymbol(symbol);

    QVector<double> samples;

    QMap<QString, int>::const_iterator i = result.constBegin();
    while (i != result.constEnd()) {
        samples.append(i.value());
        ++i;
    }

    d_barChartItem->setSamples(samples);

    double k = (0.0 + SPACING_RATIO) / MARGIN_RATIO;

    double m = (0.0 + WINDOW_WIDTH - BAR_WIDTH) / (2 + k * (result.size() - 1));
    int margin = static_cast<int>(m);
    int spacing = static_cast<int>(m * k);

    if (spacing < MIN_SPACING) spacing = MIN_SPACING;
    if (margin < MIN_MARGIN) margin = MIN_MARGIN;

    d_barChartItem->setSpacing(spacing);
    d_barChartItem->setMargin(100);
    //d_barChartItem->setLayoutPolicy(QwtPlotAbstractBarChart::FixedSampleSize);
    //d_barChartItem->setLayoutHint(BAR_WIDTH);

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

    setAxisTitle(axis2, tr("Number"));
    setAxisTitle(axis1, tr("Answers"));

    setAxisScaleDraw(axis1, new ChoicesScaleDraw(result.keys()));
    setAxisScaleDraw(axis2, new QwtScaleDraw);

    int size = result.size()-1 <= 0 ? 0: result.size()-1;
    if (size == 0) {
        setAxisAutoScale(axis1);
    }
    else {
        setAxisScale(axis1, 0, size, 1.0);
    }

    int maxsize = 0;
    foreach (int size, result.values()) {
        if (maxsize < size) maxsize = size;
    }

    setAxisScale(axis2, 0, maxsize * 1.3);

    QwtScaleDraw *scaleDraw1 = axisScaleDraw(axis1);
    scaleDraw1->enableComponent(QwtScaleDraw::Backbone, false);
    scaleDraw1->enableComponent(QwtScaleDraw::Ticks, false);

    QwtScaleDraw *scaleDraw2 = axisScaleDraw(axis2);
    scaleDraw2->enableComponent(QwtScaleDraw::Backbone, false);
    scaleDraw2->enableComponent(QwtScaleDraw::Ticks, true);

    //plotLayout()->setAlignCanvasToScales( true );
    plotLayout()->setAlignCanvasToScale(axis1, true);
    plotLayout()->setAlignCanvasToScale(axis2, false);

    plotLayout()->setCanvasMargin(0);
    updateCanvasMargins();

    replot();
}

void ICResultChart::exportChart()
{
    QwtPlotRenderer renderer;
    renderer.exportTo(this, tr("icresult.pdf"));
}
