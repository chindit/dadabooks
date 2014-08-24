#include "firstlaunch.h"
#include "ui_firstlaunch.h"

FirstLaunch::FirstLaunch(QWidget *parent) : QDialog(parent), ui(new Ui::FirstLaunch){
    ui->setupUi(this);
    insSettingsManager = new SettingsManager;
    connect(ui->buttonXML, SIGNAL(clicked()), this, SLOT(setXML()));
    connect(ui->buttonSQLite, SIGNAL(clicked()), this, SLOT(setSQLite()));
    connect(ui->buttonSQL, SIGNAL(clicked()), this, SLOT(setSQL()));
    connect(ui->buttonSkip, SIGNAL(clicked()), this, SLOT(setSkip()));
}

FirstLaunch::~FirstLaunch(){
    delete ui;
}

void FirstLaunch::setXML(){
    insSettingsManager->setSettings(Xml, true);
    QString nomFichier = this->getDirName()+"dadabooks.xml";
    insSettingsManager->setSettings(Fichier, nomFichier);
    this->close();
}

void FirstLaunch::setSQL(){
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

void FirstLaunch::setSQLite(){
    insSettingsManager->setSettings(Sqlite, true);
    insSettingsManager->setSettings(Xml, false);
    QString nomFichier = this->getDirName()+"dadabooks.db";
    insSettingsManager->setSettings(Fichier, nomFichier);
    this->close();
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
