#ifndef STORAGEDEFINITION_H
#define STORAGEDEFINITION_H

#include <QList>
#include <QMultiMap>

enum SearchConnectors { OR, AND };
enum EngineStatus { STARTING, STARTED, RUNNING, STOPPING, STOPPED, FAILURE };
struct StorageConfig {
    QString id;
    QString description;
    QVariant value;
};

class StoragePlugin
{

public:
    /**
     * Destructor
     * @brief ~StoragePlugin
     */
    virtual ~StoragePlugin() {}
    /**
     * Give parameters from settings to StorageEngine.
     * Parameters are a list of key,values
     * @brief setParameters
     * @param parameters
     */
    virtual void setParameters(QList<StorageConfig> parameters) = 0;
    /**
     * Get the list of allowed parameters from the StorageEngine
     * @brief getAllowedParameters
     * @return
     */
    virtual QList<StorageConfig> getDefaultParameters() = 0;
    /**
     * Start the StorageEngine
     * @brief start
     * @return
     */
    virtual bool start() = 0;
    /**
     * Stop the storage engine
     * @brief stop
     * @return
     */
    virtual bool stop() = 0;
    /**
     * Get last occurred error (or an empty string)
     * @brief getLastError
     * @return
     */
    virtual QString getLastError() = 0;
    /**
     * Get current storage engine status
     * @brief getStatus
     * @return
     */
    virtual EngineStatus getStatus() = 0;
    /**
     * Name of the plugin
     * Will be displayed to user
     * @brief getName
     * @return
     */
    virtual QString getName() = 0;
    /**
     * A small description of the plugin.  Will be shown to user
     * @brief getDescription
     * @return
     */
    virtual QString getDescription() = 0;
    /**
     * Version of plugin in SemVer
     * @brief getVersion
     * @return
     */
    virtual QString getVersion() = 0;
    /**
     * Unique Identifier for the plugin.
     * Will be used internally to reference the plugin
     * @brief getUUID
     * @return
     */
    virtual QString getUID() = 0;

    /**
     * Find a list of entities by list of criteria
     * @brief findBy
     * @param filters
     * @return
     */
    virtual QList<QMultiMap<QString, QString> > findBy(QMap<QString, QString> filters) = 0;
    /**
     * Find one entity by list of criteria
     * @brief findOneBy
     * @param filters
     * @return
     */
    virtual QMultiMap<QString, QString> findOneBy(QMap<QString, QString> filters) = 0;
    /**
     * Find a specific entity by ID
     * @brief find
     * @param id
     * @return
     */
    virtual QMultiMap<QString, QString> find(int id) = 0;
    /**
     * Search a list of terms in stored data
     * @brief search
     * @param terms
     * @param connector
     * @return
     */
    virtual QList<QMultiMap<QString, QString> > search(QStringList terms, SearchConnectors connector) = 0;
    /**
     * Persist/update a list of entities
     * @brief persist
     * @param entities
     */
    virtual void persist(QList<QMultiMap<QString, QString> > entities) = 0;
    /**
     * Persist/update a single entity
     * @brief persist
     * @param entity
     */
    virtual void persist(QMultiMap<QString, QString> entity) = 0;
    /**
     * Save an image and return an unique string identifier
     * @brief saveImageAsString
     * @param picture
     * @return
     */
    virtual QString saveImageAsString(QPicture picture) = 0;
    /**
     * Remove a list of entities
     * @brief remove
     * @param entities
     */
    virtual void remove(QList<QMultiMap<QString, QString> > entities) = 0;
    /**
     * Remove a single entity
     * @brief remove
     * @param entity
     */
    virtual void remove(QMultiMap<QString, QString> entity) = 0;
};

Q_DECLARE_INTERFACE(StoragePlugin, "DadaBooks.Storage.Plugin")

#endif // STORAGEDEFINITION_H
