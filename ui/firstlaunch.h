#ifndef FIRSTLAUNCH_H
#define FIRSTLAUNCH_H

#include <QDialog>
#include <QFileDialog>
#include <QFileInfo>
#include <QMessageBox>
#include <QStandardPaths>

#include "tools/logger.h"
#include "tools/pluginloader.h"
#include "tools/settings.h"
#include "ui/apidialog.h"
#include "ui/storageselectiondialog.h"
#include "config/config.h"
#include "plugins/storageplugin.h"

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
    void selectStorageMethod();

private:
    QString getDirName(bool isXML=false);
    void setConnectors();
    void updateStorageLabel();

    Ui::FirstLaunch *ui;

    Settings *settings;
    Logger *logger;
    QString currentFile;
    QString storageEngineId;
    QList<StorageConfig> storageEngineConfiguration;
};

#endif // FIRSTLAUNCH_H
