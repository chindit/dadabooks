#ifndef MOVIECOVERS_H
#define MOVIECOVERS_H

#include <QObject>
#include <QtPlugin>
#include <QEventLoop>
#include <QString>
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkRequest>
#include <QtNetwork/QNetworkReply>
#include <QGenericPlugin>
#include <QTextDocument>
#include "../../PluginTemplate.h"

class MovieCovers : public QObject, public Plugin{
    Q_INTERFACES(Plugin)
    Q_OBJECT
#if QT_VERSION >= 0x050000
    Q_PLUGIN_METADATA(IID "org.qt-project.Qt.QGenericPluginFactoryInterface" FILE "moviecovers.json")
#endif // QT_VERSION >= 0x050000

public:
    QList< QMultiMap<QString,QString> > makeSearch(QString search);
    QMultiMap<QString,QString> getBook( QString id );
    QString getName();
    QString getType();
    QString getLanguage();

private:
    QString download(QString url);
    QString ucFirst(QString chaine);
    QString convertTime(QString duree);
};

#endif // MOVIECOVERS_H
