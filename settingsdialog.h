#ifndef SETTINGSDIALOG_H
#define SETTINGSDIALOG_H

#include <QDialog>
#include <QSignalMapper>

#include "settingsmanager.h"
#include "xmlmanager.h"
#include "sqlmanager.h"

namespace Ui {
class SettingsDialog;
}

class SettingsDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit SettingsDialog(QWidget *parent = 0);
    ~SettingsDialog();

private slots:
    void accept();
    void reset();
    void manageCheck(int zone);
    void exportForGCStar();
    
private:
    void prepareUi();
    int alerteType();


    Ui::SettingsDialog *ui;
    SettingsManager *insSettingsManager;
};

#endif // SETTINGSDIALOG_H
