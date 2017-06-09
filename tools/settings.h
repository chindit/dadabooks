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
    QVariant getSetting(Setting);
    void setSetting(Setting s, QVariant v);
    void storeCollection(CollectionStorageSettings);
    CollectionStorageSettings getCollection(QString id);
    QList<CollectionStorageSettings> getCollections();
    bool reload();

private:
    void loadSettings();
    QVariant *savedSettings;
};

#endif // SETTINGS_H
