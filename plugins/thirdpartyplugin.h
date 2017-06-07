#ifndef THIRDPARTYDEFINITION_H
#define THIRDPARTYDEFINITION_H

#include <QList>

#include "config/config.h"

class ThirdPartyPlugin
{
public:
    virtual ~ThirdPartyPlugin() {}

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
     * Perform a search on third party engine based on some keywords
     * @brief makeSearch
     * @param search
     * @return
     */
    virtual QList< QMultiMap<QString,QString> > makeSearch(QString search) = 0;
    /**
     * Get product based on a string ID
     * @brief getBook
     * @param id
     * @return
     */
    virtual QMultiMap<QString,QString> getBook( QString id ) = 0;
    /**
     * Get language for given plugin
     * @brief getLanguage
     * @return
     */
    virtual QString getLanguage() = 0;
    /**
     * Get collection type of this plugin
     * @brief getRole
     * @return
     */
    virtual Collection getRole();
};

Q_DECLARE_INTERFACE(ThirdPartyPlugin, "DadaBooks.ThirdParty.Plugin")

#endif // THIRDPARTYDEFINITION_H
