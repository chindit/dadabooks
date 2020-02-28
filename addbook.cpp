#include "addbook.h"
#include "ui_addbook.h"

#include <QtWidgets/QMessageBox>
#include <QtGui/QStandardItem>

AddBook::AddBook(QWidget *parent) : QDialog(parent), ui(new Ui::AddBook){
    ui->setupUi(this);

    insEditBook = new EditBook;
    insSiteManager = new SiteManager;
    insSettingsManager = new SettingsManager;

    connect(ui->pushButton_cancel, SIGNAL(clicked()), this, SLOT(emitClose()));

    //Si c'est des films, on change
    if(insSettingsManager->getSettings(Type).toString() == "films"){
        ui->labelTitre->setText("Ajouter un nouveau film");
        ui->lineEdit_recherche->setText("Titre, acteur, directeur");
        ui->buttonManuel->setText("Ajouter manuellement un film");
        ui->comboBox_recherche->addItems(insSiteManager->getPluginList("films"));
    }
    else{
        //Ajout des sites disponibles
        ui->comboBox_recherche->addItems(insSiteManager->getPluginList());
    }
    ui->comboBoxLangue->addItems(insSiteManager->getLanguePlugins());
    this->setModal(true);
}

AddBook::~AddBook(){
    delete ui;
    delete insEditBook;
    delete insSiteManager;
    delete insSettingsManager;
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
    emit searchInternet(ui->lineEdit_recherche->text(), ui->comboBox_recherche->currentText(), ((ui->checkBoxLangue->isChecked()) ? ui->comboBoxLangue->currentText() : ""));
}

void AddBook::addManualBook(){
    insEditBook->setManual();
    insEditBook->show();
    this->close();
    return;
}

void AddBook::emitClose(){
    emit canceled();
    this->close();
    return;
}
