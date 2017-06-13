#include "storage.h"

/**
 * Constructor
 * @brief Storage::Storage
 * @param settings
 */
Storage::Storage(Settings *settings, CollectionStorageSettings collection, QWidget *parent)
{
    this->settings = settings;
    this->parent = parent;
    this->loaded = false;

    this->logger = new Logger(this->settings, this->parent);
    this->loader = new PluginLoader(this->logger, this->parent);

    // Do not load storage engine in app is not initialized
    if (this->settings->getSetting(Setting::Initialized).toBool()) {
        this->load(collection);
    }
    this->baseCollectionData = QList<BaseEntity*>();
}

/**
 * Destructorr
 * @brief Storage::~Storage
 */
Storage::~Storage()
{
    // Stop storage engine if started
    if (this->isStorageEngineLoaded()) {
        this->storageEngine->stop();
        if (!this->storageEngine->getStatus() == EngineStatus::STOPPED) {
            QMap<QString, QString> context;
            context.insert("error", this->storageEngine->getLastError());
            this->logger->error("Unable to stop storage engine.", context);
        }
    }
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
void Storage::load(CollectionStorageSettings collection)
{
    // Loading storage engine
    QString storageEngineIdentifier = (collection.storageEngine.isEmpty()) ? settings->getSetting(Setting::DefaultStorageEngine).toString()
                                                                    : collection.storageEngine;
    QList<StorageConfig> storageEngineConfig = (collection.storageEngineConfig.count() == 0) ? settings->getDefaultStorageSettings(storageEngineIdentifier)
                                                                                             : collection.storageEngineConfig;
    if (!this->loadStorageEngine(storageEngineIdentifier, storageEngineConfig)) {
        return;
    }

    // Loading collection
    if (!this->loadCollection()) {
        return;
    }
}

/**
 * Load requested storage engine
 * @brief Storage::loadStorageEngine
 * @param uid
 * @internal
 * @return
 */
bool Storage::loadStorageEngine(QString uid, QList<StorageConfig> config)
{
    // Loading engine
    if (this->loader->hasStoragePluginUid(uid)) {
        this->storageEngine = this->loader->getStoragePlugin(uid);
        this->loaded = true;
    } else if (this->loader->hasStoragePluginName(uid)) {
        this->storageEngine = this->loader->getStoragePluginByName(uid);
        this->settings->setSetting(Setting::DefaultStorageEngine, this->storageEngine->getUID());
        this->loaded = true;
    } else {
        QMap<QString, QString> context;
        context.insert("storageEngine", uid);
        this->logger->alert(QObject::tr("Le moteur de stockage n'a pas pu être chargé"), context);
        this->settings->setSetting(Setting::Initialized, false);
        return false;
    }

    // Setting parameters
    this->storageEngine->setParameters(config);

    // Starting
    this->storageEngine->start();
    if (this->storageEngine->getStatus() != EngineStatus::STARTED) {
        this->logger->error(QObject::tr("Impossible de faire démarrer le moteur de stockage.  L'erreur retournée"
                               "est la suivante: " + this->storageEngine->getLastError().toLocal8Bit()),
                            QMap<QString, QString>());
        return false;
    }
    return true;
}

/**
 * Load basic elements from the collection
 * @brief Storage::loadCollection
 * @param uid
 * @return
 */
bool Storage::loadCollection()
{
    this->baseCollectionData = this->storageEngine->getItemsList();
    return true;
}

QList<BaseEntity*> Storage::getBaseEntities()
{
    return (this->isStorageEngineLoaded()) ? this->baseCollectionData : QList<BaseEntity*>();
}

/**
 * Reload storage engine
 * @brief Storage::reload
 * @return
 */
bool Storage::reload(CollectionStorageSettings collection)
{
    // Clear stored data
    if (this->isStorageEngineLoaded()) {
        this->storageEngine->stop();
        if (!this->storageEngine->getStatus() == EngineStatus::STOPPED) {
            QMap<QString, QString> context;
            context.insert("error", this->storageEngine->getLastError());
            this->logger->error("Unable to stop storage engine.", context);
            return false;
        }
        delete this->storageEngine;
    }
    this->baseCollectionData.clear();
    this->loaded = false;

    // Re-load engine
    this->load(collection);
    return this->loaded;
}
