#include "sitemanager.h"

SiteManager* SiteManager::m_instance = 0;

SiteManager::SiteManager(QObject *parent) : QObject(parent){
    QDir pluginsDir = QDir(qApp->applicationDirPath());
        pluginsDir.cd("plugins");
        foreach(QString fileName, pluginsDir.entryList(QDir::Files)){
            QPluginLoader loader(pluginsDir.absoluteFilePath(fileName));
            QObject *plugin = loader.instance();
            if (plugin){
                Plugin * elem = qobject_cast<Plugin *>(plugin);
                if (elem){
                    pluginList << elem;
                }
            }
        }
}

SiteManager::~SiteManager(){
    //Rien de spécial à faire
}

SiteManager * SiteManager::instance()
{
    if (m_instance==0) {
        m_instance = new SiteManager();
    }
    return m_instance;
}

QStringList SiteManager::getPluginList(QString type){
    QStringList liste;
    foreach(Plugin * pg, pluginList){
        if(!type.isEmpty()){
            if(QString::compare(pg->getType(), type)==0){
                liste.append(pg->getName());
            }
        }
        else{
            liste.append(pg->getName());
        }
    }
    return liste;
}

QList< QMultiMap<QString,QString> > SiteManager::makeSearch(QString search, QString site){
    Plugin *pg = selectPlugin(site);
        if(pg){
            return pg->makeSearch(search);
        }
        QList< QMultiMap<QString,QString> > empty;
        return empty;
}

QMultiMap<QString,QString> SiteManager::getBook( QString id, QString site ){
    Plugin *pg = selectPlugin(site);
    if(pg){
        return pg->getBook(id);
    }
    QMultiMap<QString,QString> empty;
    return empty;
}

Plugin* SiteManager::selectPlugin(QString nom){
    foreach(Plugin* pg, pluginList){
        if(pg->getName() == nom){
            return pg;
        }
    }
    //ERREUR!!!
    return 0;
}
