#ifndef NV_LIVRE_H
#define NV_LIVRE_H

#include <QAbstractButton>
#include <QCheckBox>
#include <QDialog>

#include "tools/pluginloader.h"
#include "ui_additemdialog.h"

namespace Ui {
class AddItemDialog;
}

class AddItemDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit AddItemDialog(PluginLoader *loader, QWidget *parent = 0);
    ~AddItemDialog();

signals:
    void searchInternet(QString title, QString uid, QString language);
    void addManual();
    void canceled();

private slots:
    void initPluginList();
    void searchThirdParty();
    void addManualItem();
    void emitClose();

private:
    Ui::AddItemDialog *ui;
    PluginLoader *loader;
};

#endif // NV_LIVRE_H
