#include "settings.h"

Settings::Settings()
{
    this->reader = new QSettings("DadaDesktop", "dadabooks");

    this->savedSettings = new QVariant[settingsNames.size()];

    this->loadSettings();
}

/**
 * Destructor
 * @brief Settings::~Settings
 */
Settings::~Settings()
{
    delete reader;
}

/**
 * Return required setting
 * @brief Settings::getSetting
 * @param setting
 * @return
 */
QVariant Settings::getSetting(Setting s) {
    return savedSettings[s];
}

/**
 * Save new setting if needed
 * @brief Settings::setSetting
 * @param s Setting
 * @param v QVariant
 */
void Settings::setSetting(Setting s, QVariant v){
    if(savedSettings[s] != v){
        savedSettings[s] = v;
        this->reader->setValue(settingsNames.find(s)->second, savedSettings[s]);
    }
    else{
        return;
    }
}

/**
 * Store a collection into settings
 * @brief Settings::storeCollection
 * @param collection
 */
void Settings::storeCollection(CollectionStorageSettings collection)
{
    // We cannot call «this->getCollections() as we don't want a list
    // We just want the index of the collection (if stored) to overwrite data
    int storedCollections = this->reader->beginReadArray("collections");

    int index = -1;
    for (int i=0; i<storedCollections; i++) {
        this->reader->setArrayIndex(i);
        CollectionStorageSettings currentLine = this->readCollectionLine(this->reader);
        if (currentLine.id == collection.id) {
            index = i;
            break;
        }
    }
    this->reader->endArray();
    this->reader->beginWriteArray("collections");
    this->reader->setArrayIndex((index < 0) ? storedCollections : index);
    this->writeCollectionLine(collection, this->reader);
    this->reader->endArray();
    return;
}

/**
 * Get a collection from settings
 * @brief Settings::getCollection
 * @param id
 * @return
 */
CollectionStorageSettings Settings::getCollection(QString id)
{
    QList<CollectionStorageSettings> data = this->getCollections();
    for (CollectionStorageSettings datum : data) {
        if (datum.id == id) {
            return datum;
        }
    }
    // Default behavior : return empty collection
    CollectionStorageSettings settings;
    return settings;
}

/**
 * Get a list of all collections available
 * @brief Settings::getCollections
 * @return
 */
QList<CollectionStorageSettings> Settings::getCollections()
{
    int storedCollections = this->reader->beginReadArray("collections");

    QList<CollectionStorageSettings> collectionList;
    for (int i=0; i<storedCollections; i++) {
        this->reader->setArrayIndex(i);
        CollectionStorageSettings currentLine = this->readCollectionLine(this->reader);
        collectionList.append(currentLine);
    }
    this->reader->endArray();
    return collectionList;
}

/**
 * Save default storage engine config
 * @brief Settings::setDefaultStorageSettings
 * @param config
 * @param storageEngineUID
 */
void Settings::setDefaultStorageSettings(QList<StorageConfig> config, QString storageEngineUID)
{
    this->reader->beginGroup(storageEngineUID);
    for (StorageConfig datum : config) {
        this->reader->setValue(datum.id, datum.value);
    }
    this->reader->endGroup();
    return;
}

/**
 * Get default config for given storage provider
 * @brief Settings::getDefaultStorageSettings
 * @param storageEngineUID
 * @return
 */
QList<StorageConfig> Settings::getDefaultStorageSettings(QString storageEngineUID)
{
    QList<StorageConfig> config = QList<StorageConfig>();
    this->reader->beginGroup(storageEngineUID);
    if (this->reader->childKeys().count() > 0) {
        for (QString key : this->reader->childKeys()) {
            StorageConfig datum;
            datum.id = key;
            datum.value = this->reader->value(key);
            config.append(datum);
        }
    }
    this->reader->endGroup();
    return config;
}

/**
 * Load current settings
 * @brief Settings::loadSettings
 */
void Settings::loadSettings() {
    // With «find» we get an iterator.  With «second» we take its value
    savedSettings[Setting::AutoAuteur] = this->reader->value(settingsNames.find(AutoAuteur)->second, true);
    savedSettings[Setting::AutoEditeur] = this->reader->value(settingsNames.find(AutoEditeur)->second, true);
    savedSettings[Setting::Sqlite] = this->reader->value(settingsNames.find(Sqlite)->second, false);
    savedSettings[Setting::MariaDB] = this->reader->value(settingsNames.find(MariaDB)->second, false);
    savedSettings[Setting::Xml] = this->reader->value(settingsNames.find(Xml)->second, true);
    savedSettings[Setting::Fichier] = this->reader->value(settingsNames.find(Fichier)->second, "");
    savedSettings[Setting::DBHost] = this->reader->value(settingsNames.find(DBHost)->second, "localhost");
    savedSettings[Setting::DBUser] = this->reader->value(settingsNames.find(DBUser)->second, "root");
    savedSettings[Setting::DBPass] = this->reader->value(settingsNames.find(DBPass)->second, "");
    savedSettings[Setting::DBBase] = this->reader->value(settingsNames.find(DBBase)->second, "DadaBooks");
    savedSettings[Setting::Initialized] = this->reader->value(settingsNames.find(Initialized)->second, false);
    savedSettings[Setting::OpenInTab] = this->reader->value(settingsNames.find(OpenInTab)->second, true);
    savedSettings[Setting::DownPics] = this->reader->value(settingsNames.find(DownPics)->second, true);
    savedSettings[Setting::Type] = this->reader->value(settingsNames.find(Type)->second, "livres");
    savedSettings[Setting::Empruntable] = this->reader->value(settingsNames.find(Empruntable)->second, true);
    savedSettings[Setting::Verbosity] = this->reader->value(settingsNames.find(Verbosity)->second, LogLevel::WARNING);
    savedSettings[Setting::DefaultStorageEngine] = this->reader->value(settingsNames.find(DefaultStorageEngine)->second, "xml");
    savedSettings[Setting::DefaultCollection] = this->reader->value(settingsNames.find(DefaultCollection)->second, "");

    return;
}

/**
 * Force settings to be reloaded from config file
 * @brief Settings::reload
 * @return
 */
bool Settings::reload()
{
    this->loadSettings();
    return true;
}

/**
 * Read a collection from storage
 * @brief Settings::readCollectionLine
 * @internal
 * @param pointer
 * @return
 */
CollectionStorageSettings Settings::readCollectionLine(QSettings *pointer)
{
    CollectionStorageSettings collectionData;

    collectionData.id = pointer->value("id").toString();
    QString typeColl = pointer->value("type").toString();

    bool found = false;
    for (auto &type : collectionNames) {
        if (type.second == typeColl) {
            collectionData.type = type.first;
            found = true;
            break;
        }
    }

    if (!found) {
        return CollectionStorageSettings();
    }

    collectionData.storageEngine = pointer->value("storageEngine").toString();
    collectionData.useDefaultStorageSettings = pointer->value("defaultStorageSettings", false).toBool();

    pointer->beginGroup("storage");
    QStringList keys = pointer->childKeys();
    for (QString key : keys) {
        StorageConfig configLine;
        configLine.id = key;
        configLine.value = pointer->value(key);
        collectionData.storageEngineConfig.append(configLine);
    }
    pointer->endGroup();

    return collectionData;
}

/**
 * Write a collection to settings storage
 * @brief Settings::writeCollectionLine
 * @internal
 * @param data
 * @param pointer
 */
void Settings::writeCollectionLine(CollectionStorageSettings data, QSettings *pointer)
{
    pointer->setValue("id", data.id);
    pointer->setValue("type", collectionNames.find(data.type)->second);
    pointer->setValue("storageEngine", data.storageEngine);
    pointer->beginWriteArray("storage", data.storageEngineConfig.count());
    for (StorageConfig config : data.storageEngineConfig) {
        pointer->setValue(config.id, config.value);
    }
    pointer->endArray();
    return;
}
