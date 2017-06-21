#ifndef BOOK_EDIT_H
#define BOOK_EDIT_H

#include <QDialog>
#include <QFile>
#include <QListWidget>
#include <QMessageBox>
#include <QPicture>

#include "entity/movie.h"
#include "tools/logger.h"
#include "tools/networkmanager.h"
#include "tools/settings.h"
#include "tools/storage.h"
#include "tools/stringtools.h"

#include "ui_editbook.h"

namespace Ui {
class EditBook;
}

class EditBook : public QDialog
{
    Q_OBJECT
    
public:
    explicit EditBook(Storage *storage, Logger *logger, QWidget *parent, CollectionStorageSettings collection);
    explicit EditBook(Storage *storage, Logger *logger, QWidget *parent, CollectionStorageSettings collection, BaseEntity *entity);
    ~EditBook();
    void setEntity(BaseEntity *entity);
    // LEGACY
    void updateUi(QMultiMap<QString,QString> livre);
    void setId(int id);
    void setManual();

public slots:
    // LEGACY
    void updateAuteurs(int id, QString nom);
    void updateEditeurs(int id, QString nom);

private slots:
    void pictureHydratorCallback();
    void showPicture();

    // LEGACY
    void accept();
    void reject();
    void setAuteur(bool editeur=false);
    void setEditeur();
    void uploadImage();
    void uploadEbook();
    void addEtiquette();
    void etiquetteDispoToElem();
    void etiquetteElemToDispo();
    void addActeur();
    void addGenre();

signals:
    // TODO IMPLEMENT
    void editDone(int);
    void bookAdded();
    void editCanceled();
    
private:
    void setCollectionType(Collection type);
    void hydrateMovie(class Movie *movie);
    void hydratePicture(QString picture);
    void clearUI();
    void setConnectors();

    Logger *logger;
    Storage *storage;
    QWidget *parent;
    Ui::EditBook *ui;
    NetworkManager *networkManager;

    CollectionStorageSettings collection;
    BaseEntity *entity;
    QPixmap coverPicture;
    int idEdit;

    // LEGACY
    void getAuteur(QString nom);
    void getEditeur(QString nom);
    void clearAll();
    int getId(bool editeur=false);

    QDialog *dialogUploadImage;
    QLabel *labelResultatLocal;
};

#endif // BOOK_EDIT_H
