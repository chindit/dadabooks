#ifndef SITEMANAGER_H
#define SITEMANAGER_H

#include <QApplication>
#include <QDir>
#include <QList>
#include <QMap>
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
    QStringList getPluginList(QString type="livres");
    QStringList getLanguePlugins();
    QStringList getPluginsByLangue(QString langue);
    QList< QMultiMap<QString,QString> > makeSearch(QString search, QString site, QString langue, QString type);
    QMultiMap<QString,QString> getBook( QString id, QString site );
    
signals:
    
public slots:

private:
    Plugin* selectPlugin(QString nom);

    QList<Plugin *> pluginList;
    static SiteManager * m_instance;
    Plugin *pluginSite( QString nom );
    QStringList langue;
    QString currentType;
};

#endif // SITEMANAGER_H
