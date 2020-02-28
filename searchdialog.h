#ifndef SEARCHDIALOG_H
#define SEARCHDIALOG_H

#include <QtWidgets/QDialog>
#include <QtWidgets/QListWidgetItem>

#include "settingsmanager.h"
#include "sqlmanager.h"

namespace Ui {
class SearchDialog;
}

class SearchDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SearchDialog(QWidget *parent = 0);
    ~SearchDialog();

private slots:
    void accept();

private:
    Ui::SearchDialog *ui;
    SqlManager *insSqlManager;
    SettingsManager *insSettingsManager;
};

#endif // SEARCHDIALOG_H
