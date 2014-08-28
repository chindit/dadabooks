#ifndef ADD_AUTEUR_H
#define ADD_AUTEUR_H

#include <QDialog>
#include <QFileDialog>

#include "sqlmanager.h"

namespace Ui {
class AddAuteur;
}

class AddAuteur : public QDialog
{
    Q_OBJECT
    
public:
    explicit AddAuteur(QWidget *parent = 0);
    void setAuteur(int id);
    int setAuteur(QString nom);
    ~AddAuteur();

signals:
    void makeClose(int, QString);

private slots:
    void accept();
    void setImage();
    
private:
    Ui::AddAuteur *ui;
    SqlManager *insSql;
    int isEdit;
};

#endif // ADD_AUTEUR_H
