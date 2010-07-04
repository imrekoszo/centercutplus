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

public Q_SLOTS:
    void addClicked();
    void balanceModeChanged(int);
    void balanceValueChanged(int);
    void bypassStateChanged(int);
    void centerManipulationChanged(int);
    void cutModeValueChanged(int);
    void freqValueChanged(int);
    void presetSelectionChanged(int);
    void removeClicked();
};

#endif // CENTERCUTPLUSDIALOG_H
