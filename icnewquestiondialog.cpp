#include "icnewquestiondialog.h"

#include <QBitmap>
#include <QPainter>
#include <QMouseEvent>

#include "icconfig.h"

ICNewQuestionDialog::ICNewQuestionDialog(QWidget *parent) :
    QDialog(parent), onCanvas(false)
{
    setupUi(this);

    setWindowIcon(QIcon(":/images/icimage.png"));

    choiceButtonGroup->setId(tofRadioButton, TOF);
    choiceButtonGroup->setId(singleRadioButton, SINGLE_CHOICE);
    choiceButtonGroup->setId(multiRadioButton, MULTI_CHOICE);

    connect(startButton, SIGNAL(clicked()), this, SLOT(startQuestioning()));
    connect(cancelButton, SIGNAL(clicked()), this, SLOT(hide()));
}

void ICNewQuestionDialog::resizeEvent( QResizeEvent* )
{
    this->setMask( QRegion( this->rect() ) );
}

QRegion ICNewQuestionDialog::getRegion() const
{
        QPainterPath path;
        path.addRoundedRect(rect().adjusted(1, -1, -1, 0), 6, 6);
        return QRegion(path.toFillPolygon().toPolygon());
}

void ICNewQuestionDialog::updateMask()
{
    QBitmap Bitmap(size());
    Bitmap.clear();
    QPainter Painter(&Bitmap);

    Painter.setClipRegion(getRegion());
    Painter.fillRect(rect(), Qt::color1);
    Painter.end();

    setMask(Bitmap);
}

void ICNewQuestionDialog::paintEvent(QPaintEvent* event)
{
    Q_UNUSED(event);

    updateMask();

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setPen(QPen(QBrush(QColor(51, 153, 255)), 2.0));
    painter.setBrush(QBrush(QColor(252, 252, 252)));
    painter.drawRoundedRect(rect().adjusted(2,1,-2,-1), 5, 5);
}

void ICNewQuestionDialog::startQuestioning()
{
    emit newQuestion(choiceButtonGroup->checkedId(), countSpinBox->value());
    hide();
}

void ICNewQuestionDialog::mousePressEvent(QMouseEvent *e)
{
    onCanvas = true;
    lastPosition = e->globalPos();
}

void ICNewQuestionDialog::mouseMoveEvent(QMouseEvent *e)
{
    if (onCanvas == true) {
        int dx = e->globalX() - lastPosition.x();
        int dy = e->globalY() - lastPosition.y();

        lastPosition = e->globalPos();

        this->move(x()+dx, y()+dy);
        this->parentWidget()->move(this->parentWidget()->x()+dx, this->parentWidget()->y()+dy);
    }
}

void ICNewQuestionDialog::mouseReleaseEvent(QMouseEvent *e)
{
    if (onCanvas == true) onCanvas = false;
}


