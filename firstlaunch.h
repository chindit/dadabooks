#ifndef FIRSTLAUNCH_H
#define FIRSTLAUNCH_H

#include <QtWidgets/QDialog>
#include <QtWidgets/QFileDialog>
#include <QtCore/QFileInfo>
#include <QtWidgets/QLineEdit>

#include "settingsmanager.h"

namespace Ui {
class FirstLaunch;
}

class FirstLaunch : public QDialog
{
    Q_OBJECT

public:
    explicit FirstLaunch(QWidget *parent = 0);
    ~FirstLaunch();

private slots:
    void setXML();
    void setSQLite();
    void setSQL();
    void setSkip();
    void saveSQLParams();
    void accept();

private:
    QString getDirName(bool isXML=false);

    Ui::FirstLaunch *ui;
    QDialog *dialogParams;
    QLineEdit *lineEditHost;
    QLineEdit *lineEditUser;
    QLineEdit *lineEditPass;
    QLineEdit *lineEditTable;
    SettingsManager *insSettingsManager;
};

#endif // FIRSTLAUNCH_H
