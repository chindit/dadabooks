#ifndef STORAGESELECTIONDIALOG_H
#define STORAGESELECTIONDIALOG_H

#include <QDialog>
#include <QLineEdit>

#include "tools/pluginloader.h"

namespace Ui {
class StorageSelectionDialog;
}

class StorageSelectionDialog : public QDialog
{
    Q_OBJECT

public:
    explicit StorageSelectionDialog(PluginLoader *pluginLoader, QWidget *parent = 0);
    ~StorageSelectionDialog();

    QString getSelectedPlugin();

private slots:
    void updateParameters(QString plugin);

private:
    void hydrateComboBox();

    Ui::StorageSelectionDialog *ui;
    PluginLoader *pluginLoader;
};

#endif // STORAGESELECTIONDIALOG_H
