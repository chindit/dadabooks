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
                langue.append(pg->getLanguage());
            }
        }
        else{
            liste.append(pg->getName());
        }
    }
    return liste;
}

QStringList SiteManager::getLanguePlugins(){
    return langue;
}

QStringList SiteManager::getPluginsByLangue(QString langue){
    QStringList liste;
    foreach(Plugin * pg, pluginList){
        if((QString::compare(pg->getLanguage(), langue)==0) && (QString::compare(pg->getType(), currentType)==0)){
            liste.append(pg->getName());
        }
    }
    return liste;
}

QList< QMultiMap<QString,QString> > SiteManager::makeSearch(QString search, QString site, QString langue, QString type){
    QList< QMultiMap<QString,QString> > resultats;
    currentType = type;
    if(!langue.isEmpty()){
        QStringList pluginsLangue = this->getPluginsByLangue(langue);
        for(int i=0; i<pluginsLangue.size(); ++i){
            Plugin *pg = selectPlugin(pluginsLangue.at(i));
            if(pg)
                resultats.append(pg->makeSearch(search));
        }
    }
    else{
    Plugin *pg = selectPlugin(site);
        if(pg)
            resultats.append(pg->makeSearch(search));
    }
    return resultats;
}

QMultiMap<QString,QString> SiteManager::getBook( QString id, QString site ){
    Plugin *pg = selectPlugin(site);
    if(pg){
        return pg->getBook(id);
    }
    return QMultiMap<QString,QString>();
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
