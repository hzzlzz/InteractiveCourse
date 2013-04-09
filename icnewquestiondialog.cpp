#include "icnewquestiondialog.h"

#include "icconfig.h"

ICNewQuestionDialog::ICNewQuestionDialog(QWidget *parent) :
    QDialog(parent)
{
    setupUi(this);

    choiceButtonGroup->setId(tofRadioButton, TOF);
    choiceButtonGroup->setId(singleRadioButton, SINGLE_CHOICE);
    choiceButtonGroup->setId(multiRadioButton, MULTI_CHOICE);

    connect(startButton, SIGNAL(clicked()), this, SLOT(startQuestioning()));
    connect(cancelButton, SIGNAL(clicked()), this, SLOT(hide()));
}

void ICNewQuestionDialog::startQuestioning()
{
    emit newQuestion(choiceButtonGroup->checkedId(), countSpinBox->value());
    hide();
}


