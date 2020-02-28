#ifndef LENDDIALOG_H
#define LENDDIALOG_H

#include <QtCore/QDate>
#include <QtWidgets/QDialog>
#include <QtWidgets/QMessageBox>

enum lendAction { Lend, Return };

namespace Ui {
class LendDialog;
}

class LendDialog : public QDialog
{
    Q_OBJECT

signals:
    void lendCurrent(QString, QString);
    void returnCurrent(int idLend, int idItem);

public:
    explicit LendDialog(QWidget *parent = 0);
    ~LendDialog();
    void setTitle(QString title);
    void setAction(lendAction lact);
    void setBorrower(int id, int idItem, QString nom, QString email, QDate dateEmprunt);

private slots:
    void accept();
    void sendReturned();

private:
    Ui::LendDialog *ui;
    int idPret;
    int idItem;
    QString titre;
    QString courriel;
};

#endif // LENDDIALOG_H
