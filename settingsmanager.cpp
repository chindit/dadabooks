#include "settingsmanager.h"

SettingsManager::SettingsManager(QObject *parent) :
    QObject(parent)
{
    names = new QString[14];
    settings = new QVariant[14];

    names[AutoAuteur] = "AutoAuteur";
    names[AutoEditeur] = "AutoEditeur";
    names[Sqlite] = "useSQLite";
    names[MariaDB] = "useMySQL";
    names[Xml] = "useXML";
    names[Fichier] = "fichierActuel";
    names[DBHost] = "MariaDB_host";
    names[DBUser] = "MariaDB_user";
    names[DBPass] = "MariaDB_password";
    names[DBBase] = "MariaDB_base";
    names[Initialized] = "isInitialized";
    names[OpenInTab] = "ouvrirOngletPrincipal";
    names[DownPics] = "telechargerImages";
    names[Type] = "typeCollection";

    loadSettings();

}

QVariant SettingsManager::getSettings(Setting s){
    return settings[s];
}

void SettingsManager::setSettings(Setting s, QVariant v){
    if(settings[s] != v){
        settings[s] = v;
        //Enregistrement
        QSettings options("DadaBooks", "dadabooks");
        options.setValue(names[s], settings[s]);
    }
    else{
        return;
    }
}

void SettingsManager::loadSettings(){
    QSettings options("DadaBooks", "dadabooks");
    settings[AutoAuteur] = options.value(names[AutoAuteur], false);
    settings[AutoEditeur] = options.value(names[AutoEditeur], false);
    settings[Sqlite] = options.value(names[Sqlite], false);
    settings[MariaDB] = options.value(names[MariaDB], false);
    settings[Xml] = options.value(names[Xml], true);
    settings[Fichier] = options.value(names[Fichier], "");
    settings[DBHost] = options.value(names[DBHost], "localhost");
    settings[DBUser] = options.value(names[DBUser], "root");
    settings[DBPass] = options.value(names[DBPass], "");
    settings[DBBase] = options.value(names[DBBase], "DadaBooks");
    settings[Initialized] = options.value(names[Initialized], false);
    settings[OpenInTab] = options.value(names[OpenInTab], false);
    settings[DownPics] = options.value(names[DownPics], false);
    settings[Type] = options.value(names[Type], "livres");
    return;
}

//Supprime toutes les préférences
void SettingsManager::resetSettings(){
    int resultat = QMessageBox::question(0, tr("Suppression des préférences"), tr("Attention : cette action supprimera <strong>toutes</strong> vos préférences.<br />Etes-vous sur de vouloir continuer?"), QMessageBox::Yes|QMessageBox::No);
    if(resultat == QMessageBox::Yes){
        QSettings options("DadaBooks", "dadabooks");
        options.clear();
        QMessageBox::information(0, tr("Suppression effectuée"), tr("Vos préférences ont été supprimées avec succès.  La suppression sera pleinement effective après redémarrage de DadaBooks"));
    }
    return;
}

