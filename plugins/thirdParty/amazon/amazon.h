#ifndef AMAZON_H
#define AMAZON_H

#include <QObject>
#include <QtPlugin>
#include <QEventLoop>
#include <QString>
#include <QStringList>
#include <QTextDocument>
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkRequest>
#include <QtNetwork/QNetworkReply>
#include "../../PluginTemplate.h"


class Amazon : public QObject, public Plugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "org.qt-project.Qt.Plugin" FILE "amazon.json")
    Q_INTERFACES(Plugin)

public:
    QList< QMultiMap<QString,QString> > makeSearch(QString search);
    QMultiMap<QString,QString> getBook( QString id );
    QString getName();
    QString getType();
    QString getLanguage();
};

#endif // AMAZON_H
