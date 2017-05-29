#ifndef FIRSTLAUNCH_H
#define FIRSTLAUNCH_H

#include <QDialog>
#include <QFileDialog>
#include <QFileInfo>
#include <QMessageBox>

#include "settings.h"
#include "config/config.h"

namespace Ui {
    class FirstLaunch;
}

class FirstLaunch : public QDialog
{
    Q_OBJECT

public:
    explicit FirstLaunch(QWidget *parent, Settings *settings);
    ~FirstLaunch();

private slots:
    void setXML();
    void setSQLite();
    void setCloud();
    void accept();

private:
    QString getDirName(bool isXML=false);
    void setConnectors();

    Ui::FirstLaunch *ui;

    Settings *settings;
};

#endif // FIRSTLAUNCH_H
