#ifndef SITEMANAGER_H
#define SITEMANAGER_H

#include <QApplication>
#include <QDir>
#include <QList>
#include <QObject>
#include <QPluginLoader>

#include "PluginTemplate.h"

class SiteManager : public QObject
{
    Q_OBJECT
public:
    explicit SiteManager(QObject *parent = 0);
    ~SiteManager();
    static SiteManager * instance();
    QStringList getPluginList(QString type=QString());
    QList< QMultiMap<QString,QString> > makeSearch(QString search, QString site);
    QMultiMap<QString,QString> getBook( QString id, QString site );
    
signals:
    
public slots:

private:
    Plugin* selectPlugin(QString nom);

    QList<Plugin *> pluginList;
    static SiteManager * m_instance;
    Plugin *pluginSite( QString nom );
    
};

#endif // SITEMANAGER_H
