#include "searchdialog.h"
#include "ui_searchdialog.h"

SearchDialog::SearchDialog(QWidget *parent) : QDialog(parent), ui(new Ui::SearchDialog){
    ui->setupUi(this);
    ui->comboBox->addItem("OR");
    ui->comboBox->addItem("AND");

    insSettingsManager = new SettingsManager;
    if(insSettingsManager->getSettings(Xml).toBool())
        return;
    insSqlManager = new SqlManager;
    QSqlQuery resultat = insSqlManager->query("SELECT id,nom FROM etiquettes");
    while(resultat.next())
        ui->listWidget_2->addItem(resultat.record().value("nom").toString());
    if(insSettingsManager->getSettings(Type).toString() == "livres"){
        QStringList champs;
        champs << "livres.titre" << "auteurs.nom" << "livres.coauteurs" << "livres.synopsis" << "editeurs.nom";
        ui->listWidget->addItems(champs);
    }
    else{
        QStringList champs;
        champs << "titre" << "titre_original" << "directeur" << "acteurs" << "synopsis" << "genre" << "pays";
        ui->listWidget->addItems(champs);
    }
    ui->listWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->listWidget->setSelectionMode(QAbstractItemView::ExtendedSelection);
}

SearchDialog::~SearchDialog(){
    delete ui;
    if(!insSettingsManager->getSettings(Xml).toBool())
        delete insSqlManager;
    delete insSettingsManager;
}

void SearchDialog::accept(){
    if(insSettingsManager->getSettings(Xml).toBool()){
        QMessageBox::warning(this, tr("Recherche avancée non disponible"), tr("Malheureusement, la recherche avancée n'est disponible que pour une base de données type SQLite ou MySQL :/"));
        return;
    }
    //On regarde les champs sélectionnés
    QList<QListWidgetItem*> selectedFields = ui->listWidget->selectedItems();
    QList<QListWidgetItem*> selectedLabels = ui->listWidget_2->selectedItems();
    if(selectedFields.count() == 0 && selectedLabels.count() == 0){
        QMessageBox::warning(this, tr("Aucun champ sélectionné"), tr("Vous devez sélectionner au moins un champ pour effectuer la recherche"));
        return;
    }
    if(ui->lineEdit->text().isEmpty() && selectedLabels.count() == 0){
        QMessageBox::warning(this, tr("Pas de recherche"), tr("Vous devez spécifier une recherche pour continuer"));
        return;
    }
    QString req1 = "SELECT id FROM ";
    req1 += insSettingsManager->getSettings(Type).toString();
    QStringList termes = ui->lineEdit->text().trimmed().split(" ");
    if(insSettingsManager->getSettings(Type).toString() == "livres")
        req1 += "LEFT JOIN auteurs ON livres.auteur = auteurs.id LEFT JOIN editeurs ON livres.editeur = editeurs.id";
    req1 += " WHERE";

    QString temp;
    for(int i=0; i<termes.size(); ++i){
        if(termes.at(i).size() > 3){
            for(int j=0; j<selectedFields.size(); ++j){
                if(j == 0)
                    temp.append(" "+ui->comboBox->currentText()+" (");
                else
                    temp.append(" OR ");
                temp.append("("+selectedFields.at(j)->text()+" LIKE \"%");
                temp.append(termes.at(i));
                temp.append("%\")");
            }
            temp.append(")");
        }
    }
    temp = temp.right(temp.size()-(ui->comboBox->currentText().size()+1));
    req1.append(temp);
    req1.append(";");
    qDebug() << req1;
    return;
}
