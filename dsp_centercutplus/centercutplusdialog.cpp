// local headers
#include "centercutplusdialog.h"
#include "ui_centercutplusdialog.h"
#include "configuration.h"

CenterCutPlusDialog::CenterCutPlusDialog(const Configuration* configuration,
                                         QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CenterCutPlusDialog),
    _configuration(configuration)
{
    ui->setupUi(this);
}

CenterCutPlusDialog::~CenterCutPlusDialog()
{
    delete ui;
}

void CenterCutPlusDialog::changeEvent(QEvent *e)
{
    QDialog::changeEvent(e);
    switch (e->type())
    {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}

void CenterCutPlusDialog::addClicked()
{

}

void CenterCutPlusDialog::balanceModeChanged(int value)
{

}

void CenterCutPlusDialog::balanceValueChanged(int value)
{
    ui->uxBalanceLabel->setText(
            tr("%1%% - %2%%")
            .arg(50 - value)
            .arg(50 + value));
}

void CenterCutPlusDialog::bypassStateChanged(int value)
{

}

void CenterCutPlusDialog::centerManipulationChanged(int value)
{

}

void CenterCutPlusDialog::cutModeValueChanged(int value)
{
    ui->uxCutModeLabel->setText(
            tr("%1%% - %2%%")
            .arg(50 - value)
            .arg(50 + value));
}

void CenterCutPlusDialog::freqValueChanged(int value)
{

}

void CenterCutPlusDialog::presetSelectionChanged(int value)
{

}

void CenterCutPlusDialog::removeClicked()
{

}
