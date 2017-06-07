#ifndef PLUGINLOADER_H
#define PLUGINLOADER_H

#include <QDir>
#include <QException>
#include <QList>
#include <QPluginLoader>
#include <QStandardPaths>

#include "config/config.h"
#include "plugins/storageplugin.h"
#include "plugins/thirdpartyplugin.h"
#include "tools/logger.h"

class PluginLoader : public QObject
{
    Q_OBJECT
public:
    explicit PluginLoader(Logger* logger, QObject *parent = 0);
    ~PluginLoader();
    StoragePlugin* getStoragePlugin(QString uid);
    StoragePlugin* getStoragePluginByName(QString name);
    QList<StoragePlugin* > getStoragePluginList();
    bool hasStoragePluginName(QString name);
    bool hasStoragePluginUid(QString uid);

    ThirdPartyPlugin* getThirdPartyPlugin(QString uid);
    ThirdPartyPlugin* getThirdPartyPluginByName(QString name);
    QList<ThirdPartyPlugin* > getThirdPartyPluginList();
    bool hasThirdPartyPluginName(QString name);
    bool hasThirdPartyPluginUid(QString uid);

signals:

public slots:

private:
    void loadPlugins();

    QList<StoragePlugin *> storagePluginList;
    QList<ThirdPartyPlugin *> thirdPartyPluginList;

   Logger *logger;
};

#endif // PLUGINLOADER_H
