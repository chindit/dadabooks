#ifndef FIRSTLAUNCH_H
#define FIRSTLAUNCH_H

#include <QDialog>
#include <QFileDialog>
#include <QFileInfo>
#include <QMessageBox>
#include <QStandardPaths>

#include "tools/settings.h"
#include "ui/apidialog.h"
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
    void updateBreadcrumb(int current);
    void checkStorageType();
    void getStorageDir();
    void next();
    void finish();
    void accept();

private:
    QString getDirName(bool isXML=false);
    void setConnectors();
    void setupUi();

    Ui::FirstLaunch *ui;

    Settings *settings;
    QString currentDir;
};

#endif // FIRSTLAUNCH_H
