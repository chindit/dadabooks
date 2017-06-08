#include "pluginloader.h"

/**
 * Constructor
 * @brief PluginLoader::PluginLoader
 * @param parent
 */
PluginLoader::PluginLoader(Logger *logger, QObject *parent) : QObject(parent)
{
    this->logger = logger;
    this->loadPlugins();
}

/**
 * Destructor
 * @brief PluginLoader::~PluginLoader
 */
PluginLoader::~PluginLoader()
{
    // No need to delete stored plugins
}

/**
 * Autoloader for plugins
 * @brief PluginLoader::loadPlugins
 */
void PluginLoader::loadPlugins()
{
    // Load Storage plugins
    //QDir pluginsDir = QDir(QDir::currentPath() + "/" + pluginPath + "/storage");
    QDir pluginsDir = QDir(QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation) + QString("/plugins/storage"));
    foreach(QString fileName, pluginsDir.entryList(QDir::Files)){
        QPluginLoader loader(pluginsDir.absoluteFilePath(fileName));
        loader.load();
        if (!loader.isLoaded()) {
            QMap<QString, QString> context;
            context.insert("pluginName", fileName);
            context.insert("errorMsg", loader.errorString());
            context.insert("pluginDirectory", pluginsDir.absolutePath());
            this->logger->warning("Unable to load storage plugin.", context);
        } else {
            QObject *plugin = loader.instance();
            if (plugin){
                StoragePlugin * elem = qobject_cast<StoragePlugin *>(plugin);
                if (elem){
                    this->storagePluginList << elem;
                }
            }
        }
    }

    // Load thirdParty plugins
    pluginsDir = QDir(QDir::currentPath() + pluginPath + "/thirdParty");
    foreach(QString fileName, pluginsDir.entryList(QDir::Files)){
        QPluginLoader loader(pluginsDir.absoluteFilePath(fileName));
        loader.load();
        if (!loader.isLoaded()) {
            if (!loader.isLoaded()) {
                QMap<QString, QString> context;
                context.insert("pluginName", fileName);
                context.insert("errorMsg", loader.errorString());
                context.insert("pluginDirectory", pluginsDir.absolutePath());
                this->logger->warning("Unable to load storage plugin.", context);
            }
        } else {
            QObject *plugin = loader.instance();
            if (plugin){
                ThirdPartyPlugin * elem = qobject_cast<ThirdPartyPlugin *>(plugin);
                if (elem){
                    this->thirdPartyPluginList << elem;
                }
            }
        }
    }
}

/**
 * Check if given storage plugin exists
 * @brief PluginLoader::hasStoragePluginName
 * @param name
 * @return
 */
bool PluginLoader::hasStoragePluginName(QString name)
{
    for (StoragePlugin* plugin : this->storagePluginList) {
        if (plugin->getName() == name) {
            return true;
        }
    }
    return false;
}

/**
 * Check if given storage plugin exists
 * @brief PluginLoader::hasStoragePluginUid
 * @param uid
 * @return
 */
bool PluginLoader::hasStoragePluginUid(QString uid)
{
    for (StoragePlugin* plugin : this->storagePluginList) {
        if (plugin->getUID() == uid) {
            return true;
        }
    }
    return false;
}

/**
 * Return the list of loaded StoragePlugins
 * @brief PluginLoader::getStoragePluginList
 * @return
 */
QList<StoragePlugin* > PluginLoader::getStoragePluginList()
{
    return this->storagePluginList;
}

/**
 * Check if given third party plugin exists
 * @brief PluginLoader::hasThirdPartyPluginName
 * @param name
 * @return
 */
bool PluginLoader::hasThirdPartyPluginName(QString name)
{
    for (ThirdPartyPlugin* plugin : this->thirdPartyPluginList) {
        if (plugin->getName() == name) {
            return true;
        }
    }
    return false;
}

/**
 * Check if given third party plugin exists
 * @brief PluginLoader::hasThirdPartyPluginUid
 * @param uid
 * @return
 */
bool PluginLoader::hasThirdPartyPluginUid(QString uid)
{
    for (ThirdPartyPlugin* plugin : this->thirdPartyPluginList) {
        if (plugin->getUID() == uid) {
            return true;
        }
    }
    return false;
}

/**
 * Returns StoragePlugin list
 * @brief PluginLoader::getStoragePlugin
 * @param uid
 * @return
 */
StoragePlugin* PluginLoader::getStoragePlugin(QString uid)
{
    foreach (StoragePlugin* plugin, this->storagePluginList) {
        if (plugin->getUID() == uid) {
            return plugin;
        }
    }
    throw new QException();
}

/**
 * Return a specific StoragePlugin or throws an exception
 * @brief PluginLoader::getStoragePluginByName
 * @param name
 * @return
 */
StoragePlugin* PluginLoader::getStoragePluginByName(QString name)
{
    foreach (StoragePlugin* plugin, this->storagePluginList) {
        if (plugin->getName() == name) {
            return plugin;
        }
    }
    throw new QException();
}

/**
 * Return the list of available ThirdParty plugins
 * @brief PluginLoader::getThirdPartyPlugin
 * @param uid
 * @return
 */
ThirdPartyPlugin* PluginLoader::getThirdPartyPlugin(QString uid)
{
    foreach (ThirdPartyPlugin* plugin, this->thirdPartyPluginList) {
        if (plugin->getUID() == uid) {
            return plugin;
        }
    }
    throw new QException();
}

/**
 * Return selected ThirdParty plugin or throws an exception
 * @brief PluginLoader::getThirdPartyPluginByName
 * @param name
 * @return
 */
ThirdPartyPlugin* PluginLoader::getThirdPartyPluginByName(QString name)
{
    foreach (ThirdPartyPlugin* plugin, this->thirdPartyPluginList) {
        if (plugin->getName() == name) {
            return plugin;
        }
    }
    throw new QException();
}

/**
 * Return the list of loaded third party plugins
 * @brief PluginLoader::getThirdPartyPluginList
 * @return
 */
QList<ThirdPartyPlugin* > PluginLoader::getThirdPartyPluginList()
{
    return this->thirdPartyPluginList;
}
