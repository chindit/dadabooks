#ifndef FIRSTLAUNCH_H
#define FIRSTLAUNCH_H

#include <QDialog>
#include <QFileDialog>
#include <QFileInfo>
#include <QMessageBox>
#include <QStandardPaths>

#include "tools/pluginloader.h"
#include "tools/settings.h"
#include "ui/apidialog.h"
#include "config/config.h"
#include "plugins/storagedefinition.h"

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

private:
    QString getDirName(bool isXML=false);
    void setConnectors();
    void updateStorageLabel();

    Ui::FirstLaunch *ui;

    Settings *settings;
    StoragePlugin *storagePlugin;
    QString currentDir;
};

#endif // FIRSTLAUNCH_H
