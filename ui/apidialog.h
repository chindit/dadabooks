#ifndef APIDIALOG_H
#define APIDIALOG_H

#include <QDialog>
#include <QSignalMapper>

namespace Ui {
class ApiDialog;
}

class ApiDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ApiDialog(QWidget *parent = 0);
    ~ApiDialog();

private:
    void setConnectors();
    Ui::ApiDialog *ui;
    QSignalMapper *login, *registration;
};

#endif // APIDIALOG_H
