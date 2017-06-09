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

    // Do not load storage engine in app is not initialized
    if (this->settings->getSetting(Setting::Initialized).toBool()) {
        this->load();
    }
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
    QString storageEngineIdentifier = settings->getSetting(Setting::DefaultStorageEngine).toString();
    if (this->loader->hasStoragePluginUid(storageEngineIdentifier)) {
        this->storageEngine = this->loader->getStoragePlugin(storageEngineIdentifier);
        this->loaded = true;
        return;
    } else if (this->loader->hasStoragePluginName(storageEngineIdentifier)) {
        this->storageEngine = this->loader->getStoragePluginByName(storageEngineIdentifier);
        this->settings->setSetting(Setting::DefaultStorageEngine, this->storageEngine->getUID());
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
    QMap<QString, QVariant> requestedValues = QMap<QString, QVariant>();
    for (StorageConfig value : this->storageEngine->getDefaultParameters()) {
        requestedValues.insert(value.id, value.value);
    }
    QMap<QString, QVariant> parameters; // = this->settings->getGroupSettings(this->sto)
    /*QList<StorageConfig> parameters = this->settings->getGroupSettings(this->storageEngine->getUID(), this->storageEngine->getDefaultParameters());
    this->storageEngine->setParameters(parameters);*/
    this->storageEngine->start();
    if (this->storageEngine->getStatus() != EngineStatus::STARTED) {
        this->logger->error(QObject::tr("Impossible de faire démarrer le moteur de stockage.  L'erreur retournée"
                               "est la suivante: " + this->storageEngine->getLastError().toLocal8Bit()),
                            QMap<QString, QString>());
        return;
    }
}

/**
 * Reload storage engine
 * @brief Storage::reload
 * @return
 */
bool Storage::reload()
{
    this->loaded = false;
    this->load();
    return this->loaded;
}
