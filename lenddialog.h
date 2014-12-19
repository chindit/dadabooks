#ifndef LENDDIALOG_H
#define LENDDIALOG_H

#include <QDialog>
#include <QMessageBox>

enum lendAction { Lend, Return };

namespace Ui {
class LendDialog;
}

class LendDialog : public QDialog
{
    Q_OBJECT

signals:
    void lendCurrent(QString, QString);
    void returnCurrent();

public:
    explicit LendDialog(QWidget *parent = 0);
    ~LendDialog();
    void setTitle(QString title);
    void setAction(lendAction lact);

private slots:
    void accept();

private:
    Ui::LendDialog *ui;
};

#endif // LENDDIALOG_H
