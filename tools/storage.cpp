#include "storage.h"


/**
 * Base constructor
 * @brief Storage::Storage
 * @param settings
 * @param parent
 */
Storage::Storage(Settings *settings, QWidget *parent)
{
    this->settings = settings;
    this->parent = parent;
    this->loaded = false;

    this->logger = new Logger(this->settings, this->parent);
    this->loader = new PluginLoader(this->logger, this->parent);
}

/**
 * Overloaded constructor
 * @brief Storage::Storage
 * @param settings
 */
Storage::Storage(Settings *settings, QWidget *parent, CollectionStorageSettings collection)
{
    Storage(settings, parent);

    this->setCollection(collection);
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
        if (this->storageEngine->getStatus() != EngineStatus::STOPPED) {
            QMap<QString, QString> context;
            context.insert("error", this->storageEngine->getLastError());
            this->logger->error("Unable to stop storage engine.", context);
        }
    }
    delete logger;
    delete loader;
}

/**
 * Load given collection
 * @brief Storage::setCollection
 * @param collection
 * @return
 */
bool Storage::setCollection(CollectionStorageSettings collection)
{
    this->baseCollectionData = QList<BaseEntity*>();
    return this->load(collection);
}

/**
 * Check if a storage engine is loaded
 * @brief Storage::isStorageEngineLoaded
 * @return
 */
bool Storage::isStorageEngineLoaded()
{
    return (this->loaded) ? this->storageEngine->getStatus() == EngineStatus::STARTED : false;
}

/**
 * Load required storage plugin
 * @brief Storage::load
 */
bool Storage::load(CollectionStorageSettings collection)
{
    // Loading storage engine
    QString storageEngineIdentifier = (collection.storageEngine.isEmpty()) ? settings->getSetting(Setting::DefaultStorageEngine).toString()
                                                                    : collection.storageEngine;
    QList<StorageConfig> storageEngineConfig = (collection.storageEngineConfig.count() == 0) ? settings->getDefaultStorageSettings(storageEngineIdentifier)
                                                                                             : collection.storageEngineConfig;
    if (!this->loadStorageEngine(storageEngineIdentifier, storageEngineConfig)) {
        return false;
    }

    // Loading collection
    return this->loadCollection();
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
        if (this->storageEngine->getStatus() != EngineStatus::STOPPED) {
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

/**
 * Add a label to the global list
 * @brief Storage::addLabel
 * @param label
 * @return
 */
bool Storage::addLabel(QString label)
{
    if (this->isStorageEngineLoaded()) {
        return this->storageEngine->addLabel(label).contains(label);
    }
    return false;
}

/**
 * Return the list of available labels
 * @brief Storage::getAvailableLabels
 * @return
 */
QStringList Storage::getAvailableLabels()
{
    if (this->isStorageEngineLoaded()) {
        return this->storageEngine->getLabels();
    }
    return QStringList();
}
