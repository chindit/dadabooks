#ifndef ADD_EDITEUR_H
#define ADD_EDITEUR_H

#include <QDialog>

#include "sqlmanager.h"
#include "toolsmanager.h"

namespace Ui {
class AddEditeur;
}

class AddEditeur : public QDialog
{
    Q_OBJECT
    
public:
    explicit AddEditeur(QWidget *parent = 0);
    void setEditeur(int id);
    int setEditeur(QString nom);
    ~AddEditeur();

signals:
    void makeClose(int, QString);

private slots:
    void accept();
    
private:
    Ui::AddEditeur *ui;
    SqlManager *insSql;
    int isEdit;
};

#endif // ADD_EDITEUR_H
