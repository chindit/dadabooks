#include "storage.h"

/**
 * Constructor
 * @brief Storage::Storage
 * @param settings
 */
Storage::Storage(Settings *settings, QWidget *parent)
{
    this->settings = settings;
    this->parent = parent;
    this->loaded = false;

    this->logger = new Logger(this->settings, this->parent);
    this->loader = new PluginLoader(this->logger, this->parent);

    this->load();
}

/**
 * Destructorr
 * @brief Storage::~Storage
 */
Storage::~Storage()
{
    delete logger;
    delete loader;
}

/**
 * Check if a storage engine is loaded
 * @brief Storage::isStorageEngineLoaded
 * @return
 */
bool Storage::isStorageEngineLoaded()
{
    return this->loaded && this->storageEngine->getStatus() == EngineStatus::STARTED;
}

/**
 * Load required storage plugin
 * @brief Storage::load
 */
void Storage::load()
{
    // Loading storage engine
    QString storageEngineIdentifier = settings->getSetting(Setting::StorageProvider).toString();
    if (this->loader->hasStoragePluginUid(storageEngineIdentifier)) {
        this->storageEngine = this->loader->getStoragePlugin(storageEngineIdentifier);
        this->loaded = true;
        return;
    } else if (this->loader->hasStoragePluginName(storageEngineIdentifier)) {
        this->storageEngine = this->loader->getStoragePluginByName(storageEngineIdentifier);
        this->settings->setSetting(Setting::StorageProvider, this->storageEngine->getUID());
        this->loaded = true;
        return;
    } else {
        QMap<QString, QString> context;
        context.insert("storageEngine", storageEngineIdentifier);
        this->logger->alert(QObject::tr("Le moteur de stockage n'a pas pu être chargé"), context);
        this->settings->setSetting(Setting::Initialized, false);
        return;
    }

    // Starting storage engine
    QMap<QString, QVariant> parameters = this->settings->getGroupSettings(this->storageEngine->getUID(), this->storageEngine->getAllowedParameters());
    this->storageEngine->setParameters(parameters);
    this->storageEngine->start();
    if (this->storageEngine->getStatus() != EngineStatus::STARTED) {
        this->logger->error(QObject::tr("Impossible de faire démarrer le moteur de stockage.  L'erreur retournée"
                               "est la suivante: " + this->storageEngine->getLastError().toLocal8Bit()),
                            QMap<QString, QString>());
        return;
    }
}
