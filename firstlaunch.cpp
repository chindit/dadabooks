#include "firstlaunch.h"
#include "ui_firstlaunch.h"

FirstLaunch::FirstLaunch(QWidget *parent) : QDialog(parent), ui(new Ui::FirstLaunch){
    ui->setupUi(this);
    insSettingsManager = new SettingsManager;
    connect(ui->buttonXML, SIGNAL(clicked()), this, SLOT(setXML()));
    connect(ui->buttonSQLite, SIGNAL(clicked()), this, SLOT(setSQLite()));
    connect(ui->buttonSQL, SIGNAL(clicked()), this, SLOT(setSQL()));
    connect(ui->buttonSkip, SIGNAL(clicked()), this, SLOT(setSkip()));
    connect(ui->buttonGo, SIGNAL(clicked()), this, SLOT(accept()));
}

FirstLaunch::~FirstLaunch(){
    delete ui;
}

void FirstLaunch::setXML(){
    if(ui->buttonXML->isChecked()){
        ui->buttonSQL->setChecked(false);
        ui->buttonSQLite->setChecked(false);
    }
    return;
}

void FirstLaunch::setSQL(){
    if(ui->buttonSQL->isChecked()){
        ui->buttonSQLite->setChecked(false);
        ui->buttonXML->setChecked(false);
    }
    return;
}

void FirstLaunch::setSQLite(){
    if(ui->buttonSQLite->isChecked()){
        ui->buttonSQL->setChecked(false);
        ui->buttonXML->setChecked(false);
    }
    return;
}

void FirstLaunch::setSkip(){
    this->close();
}

void FirstLaunch::saveSQLParams(){
    insSettingsManager->setSettings(DBHost, lineEditHost->text());
    insSettingsManager->setSettings(DBUser, lineEditUser->text());
    insSettingsManager->setSettings(DBPass, lineEditPass->text());
    insSettingsManager->setSettings(DBBase, lineEditTable->text());

    //On ferme et on vire tout
    dialogParams->close();
    delete dialogParams;
}

QString FirstLaunch::getDirName(){
    QMessageBox::information(this, "Choisissez un dossier", "Merci de choisir un dossier dans lequel DadaBooks placera sa base de données");
    QString dirName = QFileDialog::getOpenFileName(this, "Dossier racine");
    QFileInfo dossier(dirName);
    return dossier.path()+"/";
}

void FirstLaunch::accept(){
    if(!ui->buttonSQL->isChecked() && !ui->buttonSQLite->isChecked() && !ui->buttonXML->isChecked()){
        ui->buttonXML->setChecked(true);
    }
    this->close();

    //Configurations MySQL
    if(ui->buttonSQL->isChecked()){
        insSettingsManager->setSettings(MariaDB, true);
        insSettingsManager->setSettings(Xml, false);
        this->close();
        //Fenêtre pour les paramètres
        dialogParams = new QDialog(this);
        dialogParams->setWindowTitle("Paramètres de la base de données");
        dialogParams->setWindowIcon(QIcon(":/programme/images/icone.png"));
        QLabel *labelTitre = new QLabel("Paramètres de la base");
        QLabel *labelHost = new QLabel("Serveur");
        QLabel *labelUser = new QLabel("Nom d'utilisateur");
        QLabel *labelPass = new QLabel("Mot de passe");
        QLabel *labelTable = new QLabel("Nom de la base (existante)");
        lineEditHost = new QLineEdit;
        lineEditUser = new QLineEdit;
        lineEditPass = new QLineEdit;
        lineEditTable = new QLineEdit;
        QPushButton *buttonValide = new QPushButton("GO!");
        QGridLayout *gridLayoutDialog = new QGridLayout;
        dialogParams->setLayout(gridLayoutDialog);
        gridLayoutDialog->addWidget(labelTitre, 0, 0, 1, 2, Qt::AlignHCenter);
        gridLayoutDialog->addWidget(labelHost, 1, 0);
        gridLayoutDialog->addWidget(lineEditHost, 1, 1);
        gridLayoutDialog->addWidget(labelUser, 2, 0);
        gridLayoutDialog->addWidget(lineEditUser, 2, 1);
        gridLayoutDialog->addWidget(labelPass, 3, 0);
        gridLayoutDialog->addWidget(lineEditPass, 3, 1);
        gridLayoutDialog->addWidget(labelTable, 4, 0);
        gridLayoutDialog->addWidget(lineEditTable, 4, 1);
        gridLayoutDialog->addWidget(buttonValide, 5, 0, 1, 2, Qt::AlignRight);
        connect(buttonValide, SIGNAL(clicked()), this, SLOT(saveSQLParams()));
        dialogParams->exec();
    }
    else if(ui->buttonSQLite->isChecked()){
        insSettingsManager->setSettings(Sqlite, true);
        insSettingsManager->setSettings(Xml, false);
        QString nomFichier = this->getDirName()+"dadabooks.db";
        insSettingsManager->setSettings(Fichier, nomFichier);
    }
    else if(ui->buttonXML->isChecked()){
        insSettingsManager->setSettings(Xml, true);
            QString nomFichier = this->getDirName()+"dadabooks.xml";
            insSettingsManager->setSettings(Fichier, nomFichier);
    }
    else{
        QMessageBox::critical(this, "Aucun type de collection choisi", "Aucun type de collection n'a été choisi.  Il est donc impossible de continuer");
        return;
    }

    //On gère le type
    if(ui->comboBox_Type->currentText().toLower() == "livres" || ui->comboBox_Type->currentText().toLower() == "films"){
        insSettingsManager->setSettings(Type, ui->comboBox_Type->currentText().toLower());
    }
    else{
        QMessageBox::critical(this, "Aucun type de collection choisi", "Aucun type de collection n'a été choisi.  Il est donc impossible de continuer");
        return;
    }
}
