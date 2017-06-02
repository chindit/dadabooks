#ifndef PLUGINLOADER_H
#define PLUGINLOADER_H

#include <QDir>
#include <QException>
#include <QList>
#include <QPluginLoader>
#include <QStandardPaths>

#include "config/config.h"
#include "plugins/storagedefinition.h"
#include "plugins/thirdpartydefinition.h"

class PluginLoader : public QObject
{
    Q_OBJECT
public:
    explicit PluginLoader(QObject *parent = 0);
    ~PluginLoader();
    StoragePlugin* getStoragePlugin(QString uid);
    StoragePlugin* getStoragePluginByName(QString name);

    ThirdPartyPlugin* getThirdPartyPlugin(QString uid);
    ThirdPartyPlugin* getThirdPartyPluginByName(QString name);

signals:

public slots:

private:
    void loadPlugins();

    QList<StoragePlugin *> storagePluginList;
    QList<ThirdPartyPlugin *> thirdPartyPluginList;
};

#endif // PLUGINLOADER_H
