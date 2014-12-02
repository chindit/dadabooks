#ifndef CHAPITRE_H
#define CHAPITRE_H

#include <QObject>
#include <QtPlugin>
#include <QEventLoop>
#include <QString>
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkRequest>
#include <QtNetwork/QNetworkReply>
#include "../../PluginTemplate.h"


class Chapitre : public QObject, public Plugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "org.qt-project.Qt.Plugin" FILE "chapitre.json")
    Q_INTERFACES(Plugin)
    
public:
    QList< QMultiMap<QString,QString> > makeSearch(QString search);
    QMultiMap<QString,QString> getBook( QString id );
    QString getName();
    QString getType();
    QString getLanguage();
};

//Q_EXPORT_PLUGIN2(chapitre, Chapitre)
#endif // CHAPITRE_H
