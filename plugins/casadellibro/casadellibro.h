#ifndef CASADELLIBRO_H
#define CASADELLIBRO_H

#include <QObject>
#include <QtPlugin>
#include <QEventLoop>
#include <QString>
#include <QStringList>
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkRequest>
#include <QtNetwork/QNetworkReply>
#include "../../PluginTemplate.h"


class Casadellibro : public QObject, public Plugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "org.qt-project.Qt.Plugin" FILE "casadellibro.json")
    Q_INTERFACES(Plugin)

public:
    QList< QMultiMap<QString,QString> > makeSearch(QString search);
    QMultiMap<QString,QString> getBook( QString id );
    QString getName();
    QString getType();
    QString getLanguage();

private:
    QString ucFirst(QString chaine);
};

#endif // CASADELLIBRO_H
