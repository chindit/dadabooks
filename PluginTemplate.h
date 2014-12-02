#ifndef PLUGINTEMPLATE_H
#define PLUGINTEMPLATE_H

#include <QList>
#include <QMultiMap>

class Plugin
{
public:
    virtual ~Plugin() {}

    virtual QList< QMultiMap<QString,QString> > makeSearch(QString search) = 0;
    virtual QMultiMap<QString,QString> getBook( QString id ) = 0;
    virtual QString getName() = 0;
    virtual QString getType() = 0;
    virtual QString getLanguage() = 0;
};

Q_DECLARE_INTERFACE(Plugin, "DadaBooks.Plugin")

#endif // PLUGINTEMPLATE_H
