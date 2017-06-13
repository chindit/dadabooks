#ifndef STORAGE_H
#define STORAGE_H

#include <QList>
#include <QWidget>

#include "config/config.h"
#include "entity/baseentity.h"
#include "tools/pluginloader.h"
#include "tools/settings.h"

class Storage
{
public:
    Storage(Settings *settings, CollectionStorageSettings collection, QWidget *parent);
    ~Storage();
    bool isStorageEngineLoaded();
    bool reload(CollectionStorageSettings collection);
    QList<BaseEntity*> getBaseEntities();

private:
    void load(CollectionStorageSettings collection);
    bool loadStorageEngine(QString uid, QList<StorageConfig> config);
    bool loadCollection();

    QList<BaseEntity*> baseCollectionData;
    Settings* settings;
    bool loaded;
    StoragePlugin* storageEngine;
    QWidget *parent;
    Logger *logger;
    PluginLoader *loader;
};

#endif // STORAGE_H
