#ifndef CENTERCUTPLUSDIALOG_H
#define CENTERCUTPLUSDIALOG_H

// library headers
#include <QDialog>

// forward declarations
class Configuration;

namespace Ui {
    class CenterCutPlusDialog;
}

class CenterCutPlusDialog : public QDialog {
    Q_OBJECT
public:
    CenterCutPlusDialog(const Configuration* configuration, QWidget *parent = 0);
    ~CenterCutPlusDialog();

protected:
    void changeEvent(QEvent *e);

private:
    Ui::CenterCutPlusDialog *ui;
    const Configuration * _configuration;

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
