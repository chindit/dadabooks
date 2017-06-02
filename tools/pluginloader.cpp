#include "pluginloader.h"

/**
 * Constructor
 * @brief PluginLoader::PluginLoader
 * @param parent
 */
PluginLoader::PluginLoader(QObject *parent) : QObject(parent)
{
    this->loadPlugins();
}

/**
 * Destructor
 * @brief PluginLoader::~PluginLoader
 */
PluginLoader::~PluginLoader()
{

}

/**
 * Autoloader for plugins
 * @brief PluginLoader::loadPlugins
 */
void PluginLoader::loadPlugins()
{
    // Load Storage plugins
    //QDir pluginsDir = QDir(QDir::currentPath() + "/" + pluginPath + "/storage");
    QString test = QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation);
    QDir pluginsDir = QDir(QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation) + QString("/plugins/storage"));
    foreach(QString fileName, pluginsDir.entryList(QDir::Files)){
        QPluginLoader loader(pluginsDir.absoluteFilePath(fileName));
        QObject *plugin = loader.instance();
        if (plugin){
            StoragePlugin * elem = qobject_cast<StoragePlugin *>(plugin);
            if (elem){
                this->storagePluginList << elem;
            }
        }
    }

    // Load thirdParty plugins
    pluginsDir = QDir(QDir::currentPath() + pluginPath + "/thirdParty");
    foreach(QString fileName, pluginsDir.entryList(QDir::Files)){
        QPluginLoader loader(pluginsDir.absoluteFilePath(fileName));
        QObject *plugin = loader.instance();
        if (plugin){
            ThirdPartyPlugin * elem = qobject_cast<ThirdPartyPlugin *>(plugin);
            if (elem){
                this->thirdPartyPluginList << elem;
            }
        }
    }
}

StoragePlugin* PluginLoader::getStoragePlugin(QString uid)
{
    foreach (StoragePlugin* plugin, this->storagePluginList) {
        if (plugin->getUID() == uid) {
            return plugin;
        }
    }
    throw new QException();
}

StoragePlugin* PluginLoader::getStoragePluginByName(QString name)
{
    foreach (StoragePlugin* plugin, this->storagePluginList) {
        if (plugin->getName() == name) {
            return plugin;
        }
    }
    throw new QException();
}

ThirdPartyPlugin* PluginLoader::getThirdPartyPlugin(QString uid)
{
    foreach (ThirdPartyPlugin* plugin, this->thirdPartyPluginList) {
        if (plugin->getUID() == uid) {
            return plugin;
        }
    }
    throw new QException();
}

ThirdPartyPlugin* PluginLoader::getThirdPartyPluginByName(QString name)
{
    foreach (ThirdPartyPlugin* plugin, this->thirdPartyPluginList) {
        if (plugin->getName() == name) {
            return plugin;
        }
    }
    throw new QException();
}
