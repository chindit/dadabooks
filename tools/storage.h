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
    Storage(Settings *settings, QWidget *parent);
    Storage(Settings *settings, QWidget *parent, CollectionStorageSettings collection);
    ~Storage();
    bool setCollection(CollectionStorageSettings collection);
    bool isStorageEngineLoaded();
    bool reload(CollectionStorageSettings collection);
    QList<BaseEntity*> getBaseEntities();
    bool addLabel(QString label);

private:
    bool load(CollectionStorageSettings collection);
    bool loadStorageEngine(QString uid, QList<StorageConfig> config);
    bool loadCollection();

    QList<BaseEntity*> baseCollectionData;
    Settings* settings;
    bool loaded = false;
    StoragePlugin* storageEngine;
    QWidget *parent;
    Logger *logger;
    PluginLoader *loader;
};

#endif // STORAGE_H
