#ifndef FIRSTLAUNCH_H
#define FIRSTLAUNCH_H

#include <QDialog>
#include <QFileDialog>
#include <QFileInfo>
#include <QLineEdit>

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

private:
    QString getDirName();

    Ui::FirstLaunch *ui;
    QDialog *dialogParams;
    QLineEdit *lineEditHost;
    QLineEdit *lineEditUser;
    QLineEdit *lineEditPass;
    QLineEdit *lineEditTable;
    SettingsManager *insSettingsManager;
};

#endif // FIRSTLAUNCH_H
