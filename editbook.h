#ifndef BOOK_EDIT_H
#define BOOK_EDIT_H

#include <QDialog>
#include <QMessageBox>
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkRequest>
#include <QtNetwork/QNetworkReply>

#include "addauteur.h"
#include "addediteur.h"
#include "sqlmanager.h"
#include "settingsmanager.h"
#include "xmlmanager.h"

namespace Ui {
class EditBook;
}

class EditBook : public QDialog
{
    Q_OBJECT
    
public:
    explicit EditBook(QWidget *parent = 0);
    ~EditBook();
    void updateUi(QMultiMap<QString,QString> livre);
    void setId(int id);
    void setManual();

public slots:
    void updateAuteurs(int id, QString nom);
    void updateEditeurs(int id, QString nom);

private slots:
    void accept();
    void setAuteur(bool editeur=false);
    void setEditeur();
    void uploadImage();
    void updateNote(int value);
    void uploadEbook();

signals:
    void editDone(int);
    void bookAdded();
    
private:
    void getAuteur(QString nom);
    void getEditeur(QString nom);
    int getId(bool editeur=false);
    QString guillemets(QString input);
    Ui::EditBook *ui;
    SqlManager *insSql;
    AddAuteur *insAddAuteur;
    AddEditeur *insAddEditeur;
    SettingsManager *insSettingsManager;
    XmlManager *insXml;
    int idEdit;
};

#endif // BOOK_EDIT_H
