#include "icresultwindow.h"

#include <QToolBar>
#include <QToolButton>
#include <QComboBox>

#include "icresultchart.h"

ICResultWindow::ICResultWindow(ICResultChart *chart, QWidget *parent) :
    QMainWindow(parent), d_chart(chart)
{
    setCentralWidget(d_chart);

    setWindowIcon(QIcon(":/images/icimage.png"));

    QToolBar *toolBar = new QToolBar(this);

    orientationBox = new QComboBox(toolBar);
    orientationBox->addItem(tr("Vertical"));
    orientationBox->addItem(tr("Horizontal"));
    orientationBox->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);

    btnExport = new QToolButton(toolBar);
    btnExport->setText(tr("Export"));
    btnExport->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    connect(btnExport, SIGNAL(clicked()), d_chart, SLOT(exportChart()));

    toolBar->addWidget(orientationBox);
    toolBar->addWidget(btnExport);
    addToolBar(toolBar);

    d_chart->setOrientation(orientationBox->currentIndex());
    connect(orientationBox, SIGNAL(currentIndexChanged(int)),
            d_chart, SLOT(setOrientation(int)));

    setAttribute(Qt::WA_DeleteOnClose);
}

ICResultWindow::~ICResultWindow()
{
    disconnect(btnExport, SIGNAL(clicked()), d_chart, SLOT(exportChart()));
    disconnect(orientationBox, SIGNAL(currentIndexChanged(int)),
            d_chart, SLOT(setOrientation(int)));
    delete d_chart;
}
