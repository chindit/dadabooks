#ifndef STORAGEDEFINITION_H
#define STORAGEDEFINITION_H

#include <QList>
#include <QMultiMap>

#include "plugins/abstractplugin.h"

class StoragePlugin: AbstractPlugin
{
public:
    virtual ~StoragePlugin() {}

    virtual QList< QMultiMap<QString,QString> > makeSearch(QString search) = 0;
    virtual QMultiMap<QString,QString> getBook( QString id ) = 0;
    virtual QString getLanguage() = 0;
};

Q_DECLARE_INTERFACE(StoragePlugin, "DadaBooks.Storage.Plugin")

#endif // STORAGEDEFINITION_H
