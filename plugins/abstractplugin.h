#ifndef ABSTRACTPLUGIN_H
#define ABSTRACTPLUGIN_H

/**
 * This class MUST be extended by all plugins.
 * It contains basic information for the interaction with DadaBooks
 * @brief The AbstractPlugin class
 */
class AbstractPlugin
{
    /**
     * Name of the plugin
     * Will be displayed to user
     * @brief getName
     * @return
     */
    virtual QString getName() = 0;
    /**
     * Unique Identifier for the plugin.
     * Will be used internally to reference the plugin
     * @brief getUUID
     * @return
     */
    virtual QString getUID() = 0;
    /**
     * Return the type of the plugin.
     * This allows DadaBooks to know what the plugin does exactly
     * (Get data from external sources, storage engine,… )
     * @brief getType
     * @return
     */
    virtual QString getType() = 0;
};

#endif // ABSTRACTPLUGIN_H
