#include "sqlmanager.h"

SqlManager::SqlManager(){
    name.start();
    insManager = new SettingsManager;
    //Constructeur -> On vérifie si tout est OK
    this->connect();

    if(insManager->getSettings(Type).toString() == "films"){
        //Vérification des tables
        QString requete = "SELECT * FROM films LIMIT 1";
        if(instance.exec(requete).lastError().isValid()){
            requete = "CREATE TABLE IF NOT EXISTS `etiquettes` (`id` INTEGER PRIMARY KEY, `nom` VARCHAR(50) NOT NULL)";
            instance.exec(requete); qDebug() << instance.lastError();
            requete = "CREATE TABLE IF NOT EXISTS `liste_etiquettes` (`id` INTEGER PRIMARY KEY, `id_livre` INTEGER NOT NULL, `id_etiquette` INTEGER NOT NULL)";
            instance.exec(requete);
            requete = "CREATE TABLE IF NOT EXISTS `films` (`id` INTEGER PRIMARY KEY,`titre` VARCHAR(250) NOT NULL,`titre_original` VARCHAR(250) NOT NULL,`directeur` VARCHAR(100) NOT NULL,`acteurs` VARCHAR(1000) NOT NULL,`synopsis` TEXT NOT NULL,`annee` YEAR(4) NOT NULL,`duree` INTEGER NOT NULL,`genre` VARCHAR(500) NOT NULL,`pays` VARCHAR(150) NOT NULL,`langue` VARCHAR(100) NOT NULL,`classement` VARCHAR(100) NOT NULL,`sous_titres` VARCHAR(200) NOT NULL,`commentaire` TEXT NOT NULL,`note` INTEGER NOT NULL,`jaquette` VARCHAR(150) NOT NULL,`empruntable` TINYINT NOT NULL DEFAULT '1',`prete` TINYINT NOT NULL DEFAULT '0',`vu` TINYINT NOT NULL DEFAULT '0',`fichier` TINYINT NOT NULL DEFAULT '0',`emplacement` VARCHAR(250) NOT NULL,`qualite` VARCHAR(100) NOT NULL)";
            instance.exec(requete);
            //if(insManager->getSettings(MariaDB).toBool()){
                instance.exec("ALTER TABLE `etiquettes` CHANGE `id` `id` INT(3) NOT NULL AUTO_INCREMENT;");
                instance.exec("ALTER TABLE `liste_etiquettes` CHANGE `id` `id` INT(5) NOT NULL AUTO_INCREMENT;");
                instance.exec("ALTER TABLE `films` CHANGE `id` `id` INT(11) NOT NULL AUTO_INCREMENT;");
            /*}
            else{
                instance.exec("ALTER TABLE `etiquettes` CHANGE `id` `id` INT(3) NOT NULL AUTOINCREMENT;");
                instance.exec("ALTER TABLE `liste_etiquettes` CHANGE `id` `id` INT(5) NOT NULL AUTOINCREMENT;");
                instance.exec("ALTER TABLE `films` CHANGE `id` `id` INT(11) NOT NULL AUTOINCREMENT;");
            }*/
        }
    }
    else{
        //Vérification des tables
        QString requete = "SELECT * FROM livres LIMIT 1";
        if(instance.exec(requete).lastError().isValid()){
            //On crée les tables
            requete = "CREATE TABLE IF NOT EXISTS auteurs (id INTEGER PRIMARY KEY, nom VARCHAR(250),naissance YEAR(4) NOT NULL,mort YEAR(4) NOT NULL,`biographie` TEXT NOT NULL,`photo` VARCHAR(75) NOT NULL);";
            instance.exec(requete);
            requete = "CREATE TABLE IF NOT EXISTS `editeurs` (`id` INTEGER PRIMARY KEY NOT NULL,`nom` VARCHAR(150) NOT NULL,`adresse` VARCHAR(250) NOT NULL,`site` VARCHAR(250) NOT NULL);";
            instance.exec(requete);
            requete = "CREATE TABLE IF NOT EXISTS `livres` (`id` INTEGER PRIMARY KEY NOT NULL,`titre` VARCHAR(255) NOT NULL,`ISBN` VARCHAR(13) NOT NULL,`auteur` INTEGER NOT NULL,`coauteurs` VARCHAR(50) NOT NULL,`synopsis` TEXT NOT NULL,`couverture` VARCHAR(100) NOT NULL,`editeur` INTEGER NOT NULL,`annee` YEAR(4) NOT NULL,`pages` INTEGER NOT NULL,`edition` INTEGER NOT NULL DEFAULT '1',`langue` VARCHAR(25) NOT NULL,`classement` VARCHAR(255) NOT NULL,`exemplaires` INTEGER NOT NULL DEFAULT '1',`commentaire` TEXT NOT NULL,`lu` TINYINT NOT NULL,`note` INTEGER NOT NULL DEFAULT '1',`empruntable` TINYINT NOT NULL,`pret` TINYINT NOT NULL,`ebook` TINYINT NOT NULL,`emplacement` VARCHAR(255) NOT NULL);";
            instance.exec(requete);
            requete = "CREATE TABLE IF NOT EXISTS `etiquettes` (`id` INTEGER PRIMARY KEY NOT NULL, `nom` VARCHAR(50) NOT NULL)";
            instance.exec(requete);
            requete = "CREATE TABLE IF NOT EXISTS `liste_etiquettes` (`id` INTEGER PRIMARY KEY NOT NULL, `id_livre` INTEGER NOT NULL, `id_etiquette` INTEGER NOT NULL)";
            instance.exec(requete);
            //On ajoute les clés et les auto-increment
            instance.exec("ALTER TABLE `auteurs` CHANGE `id` `id` INT(11) NOT NULL AUTO_INCREMENT;");
            instance.exec("ALTER TABLE `editeurs` CHANGE `id` `id` INT(11) NOT NULL AUTO_INCREMENT;");
            instance.exec("ALTER TABLE `etiquettes` CHANGE `id` `id` INT(3) NOT NULL AUTO_INCREMENT;");
            instance.exec("ALTER TABLE `liste_etiquettes` CHANGE `id` `id` INT(5) NOT NULL AUTO_INCREMENT;");
            instance.exec("ALTER TABLE `livres` CHANGE `id` `id` INT(11) NOT NULL AUTO_INCREMENT;");
        }
    }
}

QSqlQuery SqlManager::query(QString req){
    if(!instance.isOpen())
        this->connect();
    QSqlQuery temp = instance.exec(req);
    if(!temp.lastError().isValid())
        return temp;
    else{
        qDebug() << temp.lastQuery();
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
        instance = QSqlDatabase::addDatabase("QSQLITE", "sqlite_at_"+QString::number(name.nsecsElapsed()));
        QString chemin_repertoire = insManager->getSettings(Fichier).toString();
        instance.setDatabaseName(chemin_repertoire);
    }
    else if(insManager->getSettings(MariaDB).toBool()){
        instance = QSqlDatabase::addDatabase("QMYSQL", "mysql_at_"+QString::number(name.nsecsElapsed()));
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

QList<QMultiMap<QString, QString> > SqlManager::convertToXML(QSqlQuery requete){
    QStringList champs, booleanFields;
    QList<QMultiMap<QString, QString> > data;
    QMap<QString, QString> conversion;
    conversion.insert("sous_titres", "sousTitres");
    conversion.insert("titre_original", "titreOriginal");
    conversion.insert("jaquette", "couverture");
    conversion.insert("nom_editeur", "editeur");
    conversion.insert("nom", "auteur");

    booleanFields << "empruntable" << "prete" << "vu" << "lu" << "fichier" << "ebook";

    for(int i=0; i<requete.record().count(); ++i){
        champs.append(requete.record().fieldName(i));
    }

    while(requete.next()){ //Parcours des résultats
        QMultiMap<QString, QString> item;
        for(int i=0; i<champs.count(); ++i){ //Parcours des champs
            if(booleanFields.contains(champs.at(i)))
                item.insert(champs.at(i), ((requete.record().value(champs.at(i)).toBool()) ? "True" : "False"));
            else{
                if(conversion.contains(champs.at(i))){
                    item.insert(conversion.value(champs.at(i)), ToolsManager::stripSlashes(requete.record().value(champs.at(i)).toString()));
                }
                else{
                    item.insert(champs.at(i), ToolsManager::stripSlashes(requete.record().value(champs.at(i)).toString()));
                }
            }
        }
        data.append(item);
    }
    return data;
}

QStringList SqlManager::getListEtiquettes(){
    QSqlQuery liste = this->query("SELECT nom FROM etiquettes");
    QStringList etiquettes;
    while(liste.next()){
        etiquettes.append(liste.record().value("nom").toString());
    }
    return etiquettes;
}
