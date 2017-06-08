#ifndef STORAGE_H
#define STORAGE_H

#include <QWidget>

#include "config/config.h"
#include "tools/pluginloader.h"
#include "tools/settings.h"

class Storage
{
public:
    Storage(Settings *settings, QWidget *parent);
    ~Storage();
    bool isStorageEngineLoaded();
    bool reload();

private:
    void load();

    Settings* settings;
    bool loaded;
    StoragePlugin* storageEngine;
    QWidget *parent;
    Logger *logger;
    PluginLoader *loader;
};

#endif // STORAGE_H
