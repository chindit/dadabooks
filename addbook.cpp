#include "addbook.h"
#include "ui_addbook.h"

#include <QMessageBox>

AddBook::AddBook(QWidget *parent) : QDialog(parent), ui(new Ui::AddBook){
    ui->setupUi(this);

    insEditBook = new EditBook;
    insSiteManager = new SiteManager;
    //Ajout des sites disponibles
    ui->comboBox_recherche->addItems(insSiteManager->getPluginList());
}

AddBook::~AddBook(){
    delete ui;
    delete insEditBook;
    delete insSiteManager;
}

void AddBook::addInternetBook(){
    if(ui->lineEdit_recherche->text().isEmpty()){
        QMessageBox::warning(this, tr("Chaîne vide"), tr("Impossible de démarrer la recherche, vous n'avez spécifié aucun terme"));
        return;
    }
    if(ui->comboBox_recherche->currentText().isEmpty()){
        QMessageBox::warning(this, tr("Pas de site"), tr("Vous n'avez spécifié aucun site sur lequel effectuer la recherche"));
        return;
    }
    //Si on est ici, c'est que tout est OK.
    emit searchInternet(ui->lineEdit_recherche->text(), ui->comboBox_recherche->currentText());
}

void AddBook::addManualBook(){
    insEditBook->setManual();
    insEditBook->show();
    return;
}
