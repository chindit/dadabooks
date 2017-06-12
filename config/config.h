#ifndef CONFIG_H
#define CONFIG_H

#include <map>

#include <QPair>
#include <QString>
#include <QVariant>

#include "plugins/storageplugin.h"

// Settings enumeration
enum Setting { AutoAuteur, AutoEditeur, Sqlite, MariaDB, DBHost, DBUser, DBPass, DBBase, Type, Xml, Fichier, Initialized, OpenInTab, DownPics, Empruntable, Verbosity, DefaultStorageEngine,
             DefaultCollection, Collections};
// NOTE
// «Type» is left in «Settings» for compatibility build.  It will be removed as soon as possible in favor of CollectionSetting::Type
// «TypeC» will then be renamed to «Type»

enum CollectionSetting { TypeC, StorageEngine, File, UseDefaultStorageSettings };

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
    {Empruntable, "isEmpruntable"},
    {Verbosity, "verbosity"},
    {DefaultStorageEngine, "defaultStorageEngine"},
    {DefaultCollection, "defaultCollection"},
    {Collections, "collectionList"}
};

// Settings variables for collections
const std::map<CollectionSetting, const char*> collectionSettingsNames {
    {TypeC, "typeCollection"},
    {StorageEngine, "storageEngine"},
    {File, "file"},
    {UseDefaultStorageSettings, "defaultSettings"}
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
const QString pluginPath = QString("plugins");

struct CollectionStorageSettings {
    QString id;
    Collection type;
    QString storageEngine;
    bool useDefaultStorageSettings;
    QList<StorageConfig> storageEngineConfig;
};

#endif // CONFIG_H
