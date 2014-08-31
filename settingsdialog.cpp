#include "settingsdialog.h"
#include "ui_settingsdialog.h"

SettingsDialog::SettingsDialog(QWidget *parent) : QDialog(parent), ui(new Ui::SettingsDialog){
    ui->setupUi(this);
    insSettingsManager = new SettingsManager;
    this->prepareUi();

    QSignalMapper *map1, *map2, *map3;
    map1 = new QSignalMapper;
    map2 = new QSignalMapper;
    map3 = new QSignalMapper;
    connect(ui->groupBox_mysql, SIGNAL(clicked()), map1, SLOT(map()));
    map1->setMapping(ui->groupBox_mysql, 2);
    connect(map1, SIGNAL(mapped(int)), this, SLOT(manageCheck(int)));
    connect(ui->groupBox_sqlite, SIGNAL(clicked()), map2, SLOT(map()));
    map2->setMapping(ui->groupBox_sqlite, 1);
    connect(map2, SIGNAL(mapped(int)), this, SLOT(manageCheck(int)));
    connect(ui->groupBox_xml, SIGNAL(clicked()), map3, SLOT(map()));
    map3->setMapping(ui->groupBox_xml, 3);
    connect(map3, SIGNAL(mapped(int)), this, SLOT(manageCheck(int)));
}

SettingsDialog::~SettingsDialog(){
    delete ui;
    delete insSettingsManager;
}

void SettingsDialog::accept(){
    //SQLite
    insSettingsManager->setSettings(Sqlite, ui->groupBox_sqlite->isChecked());
    insSettingsManager->setSettings(AutoAuteur, ui->checkBox_auteurs->isChecked());
    insSettingsManager->setSettings(AutoEditeur, ui->checkBox_editeurs->isChecked());
    //XML
    insSettingsManager->setSettings(Xml, ui->groupBox_xml->isChecked());
    //MariaDB
    insSettingsManager->setSettings(MariaDB, ui->groupBox_mysql->isChecked());
    if(ui->groupBox_mysql->isChecked()){
        insSettingsManager->setSettings(DBHost, ui->lineEdit_host->text());
        insSettingsManager->setSettings(DBUser, ui->lineEdit_user->text());
        insSettingsManager->setSettings(DBPass, ui->lineEdit_pass->text());
        insSettingsManager->setSettings(DBBase, ui->lineEdit_base->text());
    }
    insSettingsManager->setSettings(OpenInTab, ui->checkBox_ongletPrincipal->isChecked());
    this->close();
}

void SettingsDialog::reset(){
    insSettingsManager->resetSettings();
    return;
}

void SettingsDialog::prepareUi(){
    //Update values
    //SQLite
    ui->groupBox_sqlite->setChecked(insSettingsManager->getSettings(Sqlite).toBool());
    ui->checkBox_auteurs->setChecked(insSettingsManager->getSettings(AutoAuteur).toBool());
    ui->checkBox_editeurs->setChecked(insSettingsManager->getSettings(AutoEditeur).toBool());
    //XML
    ui->groupBox_xml->setChecked(insSettingsManager->getSettings(Xml).toBool());
    //MySQL
    ui->groupBox_mysql->setChecked(insSettingsManager->getSettings(MariaDB).toBool());
    if(!insSettingsManager->getSettings(Xml).toBool())
        ui->horizontalLayout_3->setEnabled(false);
    if(insSettingsManager->getSettings(MariaDB).toBool()){
        ui->lineEdit_base->setText(insSettingsManager->getSettings(DBBase).toString());
        ui->lineEdit_host->setText(insSettingsManager->getSettings(DBHost).toString());
        ui->lineEdit_user->setText(insSettingsManager->getSettings(DBUser).toString());
        ui->lineEdit_pass->setText(insSettingsManager->getSettings(DBPass).toString());
    }
    ui->checkBox_ongletPrincipal->setChecked(insSettingsManager->getSettings(OpenInTab).toBool());
    return;
}

void SettingsDialog::manageCheck(int zone){
    if(zone == 1){
        ui->groupBox_mysql->setChecked(false);
        ui->groupBox_xml->setChecked(false);
        ui->horizontalLayout_3->setEnabled(false);
    }
    else if(zone == 2){
        ui->groupBox_sqlite->setChecked(false);
        ui->groupBox_xml->setChecked(false);
        ui->horizontalLayout_3->setEnabled(false);
    }
    else{
        ui->groupBox_sqlite->setChecked(false);
        ui->groupBox_mysql->setChecked(false);
        ui->horizontalLayout_3->setEnabled(true);
    }
    return;
}

void SettingsDialog::exportForGCStar(){
    XmlManager manager;
    QList<QMultiMap<QString, QString> > base;
    if(insSettingsManager->getSettings(Xml).toBool()){
        base = manager.readBase();
    }
    else{
        //On crée manuellement la liste
    SqlManager sql;
        QSqlQuery requete = sql.query("SELECT livres.id, livres.titre, livres.ISBN, livres.coauteurs, livres.synopsis, livres.couverture, livres.pages, livres.edition, livres.langue, livres.classement, livres.exemplaires, livres.commentaire, livres.lu, livres.note, livres.empruntable, livres.pret, livres.annee, auteurs.nom, editeurs.nom AS nom_editeur FROM livres LEFT JOIN auteurs ON livres.auteur = auteurs.id LEFT JOIN editeurs ON livres.editeur = editeurs.id ORDER BY id");
        while(requete.next()){
            QMultiMap <QString, QString> livre;
            livre.insert("id", requete.record().value("id").toString());
            livre.insert("titre", requete.record().value("titre").toString());
            livre.insert("isbn", requete.record().value("isbn").toString());
            livre.insert("synopsis", requete.record().value("synopsis").toString());
            livre.insert("couverture", requete.record().value("couverture").toString());
            livre.insert("pages", requete.record().value("pages").toString());
            livre.insert("edition", requete.record().value("edition").toString());
            livre.insert("langue", requete.record().value("langue").toString());
            livre.insert("classement", requete.record().value("classement").toString());
            livre.insert("commentaire", requete.record().value("commentaire").toString());
            livre.insert("editeur", requete.record().value("nom_editeur").toString());
            livre.insert("annee", requete.record().value("annee").toString());
            livre.insert("lu", requete.record().value("lu").toBool() ? "True" : "False");
            base.append(livre);
        }
    }
    manager.exportCurrentAsGCStar(base);
    return;
}
