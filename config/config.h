#ifndef CONFIG_H
#define CONFIG_H

#include <map>

#include <QString>

// Settings enumeration
enum Setting { AutoAuteur, AutoEditeur, Sqlite, MariaDB, DBHost, DBUser, DBPass, DBBase, Xml, Fichier, Initialized, OpenInTab, DownPics, Type, Empruntable };

// Settings variables
const std::map<Setting, const char*> settingsNames {
    {AutoAuteur, "AutoAuteur"},
    {AutoEditeur, "AutoEditeur"},
    {Sqlite, "useSQLite"},
    {MariaDB, "useMySQL"},
    {Xml, "useXML"},
    {Fichier, "fichierActuel"},
    {DBHost, "MariaDB_host"},
    {DBUser, "MariaDB_user"},
    {DBPass, "MariaDB_password"},
    {DBBase, "MariaDB_base"},
    {Initialized, "isInitialized"},
    {OpenInTab, "ouvrirOngletPrincipal"},
    {DownPics, "telechargerImages"},
    {Type, "typeCollection"},
    {Empruntable, "isEmpruntable"}
};

// Collection type handled by DadaBooks
enum Collection { Book, Movie };

// Collection variables
const std::map<Collection, const char*> collectionNames {
    {Book, "livres"},
    {Movie, "films"}
};

// Log level
enum LogLevel { DEBUG, INFO, NOTICE, WARNING, ERROR, CRITICAL, ALERT, EMERGENCY };

// Log level names
const std::map<LogLevel, const char*> logLevelNames {
    {DEBUG, "DEBUG"},
    {INFO, "INFO"},
    {NOTICE, "NOTICE"},
    {WARNING, "WARNING"},
    {ERROR, "ERROR"},
    {CRITICAL, "CRITICAL"},
    {ALERT, "ALERT"},
    {EMERGENCY, "EMERGENCY"}
};

const QString LogFilename = QString("dadabooks.log");

#endif // CONFIG_H
