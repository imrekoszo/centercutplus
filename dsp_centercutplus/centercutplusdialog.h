#ifndef CENTERCUTPLUSDIALOG_H
#define CENTERCUTPLUSDIALOG_H

#include <QDialog>

namespace Ui {
    class CenterCutPlusDialog;
}

class CenterCutPlusDialog : public QDialog {
    Q_OBJECT
public:
    CenterCutPlusDialog(QWidget *parent = 0);
    ~CenterCutPlusDialog();

protected:
    void changeEvent(QEvent *e);

private:
    Ui::CenterCutPlusDialog *ui;
};

#endif // CENTERCUTPLUSDIALOG_H
