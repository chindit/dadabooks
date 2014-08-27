#include "sqlmanager.h"

SqlManager::SqlManager(){
    insManager = new SettingsManager;
    //Constructeur -> On vérifie si tout est OK
    this->connect();

    //Vérification des tables
    QString requete = "SELECT * FROM livres";
    if(instance.exec(requete).lastError().isValid()){
        //On crée les tables
        requete = "CREATE TABLE IF NOT EXISTS auteurs (id INTEGER PRIMARY KEY, nom VARCHAR(250),naissance YEAR(4) NOT NULL,mort YEAR(4) NOT NULL,`biographie` TEXT NOT NULL,`photo` VARCHAR(75) NOT NULL);";
        instance.exec(requete);
        requete = "CREATE TABLE IF NOT EXISTS `editeurs` (`id` INTEGER PRIMARY KEY NOT NULL,`nom` VARCHAR(150) NOT NULL,`adresse` VARCHAR(250) NOT NULL,`site` VARCHAR(250) NOT NULL);";
        instance.exec(requete);
        requete = "CREATE TABLE IF NOT EXISTS `livres` (`id` INTEGER PRIMARY KEY NOT NULL,`titre` VARCHAR(255) NOT NULL,`ISBN` VARCHAR(13) NOT NULL,`auteur` INTEGER NOT NULL,`coauteurs` VARCHAR(50) NOT NULL,`synopsis` TEXT NOT NULL,`couverture` VARCHAR(100) NOT NULL,`editeur` INTEGER NOT NULL,`annee` YEAR(4) NOT NULL,`pages` INTEGER NOT NULL,`edition` INTEGER NOT NULL DEFAULT '1',`langue` VARCHAR(25) NOT NULL,`classement` VARCHAR(255) NOT NULL,`exemplaires` INTEGER NOT NULL DEFAULT '1',`commentaire` TEXT NOT NULL,`lu` TINYINT NOT NULL,`note` INTEGER NOT NULL DEFAULT '1',`empruntable` TINYINT NOT NULL,`pret` TINYINT NOT NULL,`ebook` TINYINT NOT NULL,`emplacement` VARCHAR(255) NOT NULL);";
        instance.exec(requete);
    }
}

QSqlQuery SqlManager::query(QString req){
    if(!instance.isOpen())
        this->connect();
    QSqlQuery temp = instance.exec(req);
    if(!temp.lastError().isValid())
        return temp;
    else{
        QMessageBox::critical(0, "Erreur lors de la requête", "La requête n'a pu être effectuée.  Voici le message retourné : \n"+temp.lastError().text());
        return temp;
    }
}

SqlManager::~SqlManager (){
    if(instance.isValid()){
        this->disconnect();
        delete insManager;
    }
    return;
}

void SqlManager::connect(){
    if(insManager->getSettings(Sqlite).toBool()){
        instance = QSqlDatabase::addDatabase("QSQLITE", "instance_sql");
        QString chemin_repertoire = insManager->getSettings(Fichier).toString();
        instance.setDatabaseName(chemin_repertoire);
    }
    else if(insManager->getSettings(MariaDB).toBool()){
        instance = QSqlDatabase::addDatabase("QMYSQL");
        instance.setHostName(insManager->getSettings(DBHost).toString());
        instance.setUserName(insManager->getSettings(DBUser).toString());
        instance.setPassword(insManager->getSettings(DBPass).toString());
        instance.setDatabaseName(insManager->getSettings(DBBase).toString());
    }
    if(!instance.open()){
        QMessageBox::critical(0, "Sql Error", QString("Impossible d'ouvrir la base de données\nL'erreur retournée est la suivante %1").arg(instance.lastError().text()));
        exit(EXIT_FAILURE);
    }
}

void SqlManager::disconnect(){
    if(instance.isOpen())
        instance.close();
    return;
}
