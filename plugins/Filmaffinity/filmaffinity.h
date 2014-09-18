#ifndef FILMAFFINITY_H
#define FILMAFFINITY_H

#include <QObject>
#include <QtPlugin>
#include <QEventLoop>
#include <QMultiMap>
#include <QString>
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkRequest>
#include <QtNetwork/QNetworkReply>
#include "../../PluginTemplate.h"

class Filmaffinity : public QObject, public Plugin
{
    Q_INTERFACES(Plugin)
    Q_OBJECT
#if QT_VERSION >= 0x050000
    Q_PLUGIN_METADATA(IID "org.qt-project.Qt.QGenericPluginFactoryInterface" FILE "Filmaffinity.json")
#endif // QT_VERSION >= 0x050000

public:
    QList< QMultiMap<QString,QString> > makeSearch(QString search);
    QMultiMap<QString,QString> getBook( QString id );
    QString getName();
    QString getType();
};

#endif // FILMAFFINITY_H
