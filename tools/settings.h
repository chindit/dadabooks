#ifndef SETTINGS_H
#define SETTINGS_H

#include <QApplication>
#include <QSettings>

#include "config/config.h"

class Settings
{
public:
    Settings();
    ~Settings();
    QVariant getSetting(Setting s);
    void setSetting(Setting s, QVariant v);
    void storeCollection(CollectionStorageSettings collection);
    QList<StorageConfig> getDefaultStorageSettings(QString storageEngineUID);
    void setDefaultStorageSettings(QList<StorageConfig> config, QString storageEngineUID);
    CollectionStorageSettings getCollection(QString id);
    QList<CollectionStorageSettings> getCollections();
    bool reload();

private:
    CollectionStorageSettings readCollectionLine(QSettings *pointer);
    void writeCollectionLine(CollectionStorageSettings data, QSettings *pointer);
    void loadSettings();
    QVariant *savedSettings;
    QSettings *reader;
};

#endif // SETTINGS_H
