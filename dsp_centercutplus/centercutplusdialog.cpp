#include "centercutplusdialog.h"
#include "ui_centercutplusdialog.h"

CenterCutPlusDialog::CenterCutPlusDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CenterCutPlusDialog)
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
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}
