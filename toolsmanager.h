#ifndef TOOLSMANAGER_H
#define TOOLSMANAGER_H

#include <math.h>
#include <QDir>
#include <QEventLoop>
#include <QMessageBox>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QPainter>
#include <QPixmap>
#include <QtPrintSupport/QPrinter>
#include <QString>
#include <QTextDocument>

class ToolsManager
{
public:
    ToolsManager();
    static QString downloadFile(QString file, QDir dossier);
    static QPixmap getPixmapFromString(QString adresse);
    static void exportMovieList(QList<QMultiMap<QString, QString> > base, QString dir, bool pdf=false);

private:
    QByteArray downloadLink(QString url);
    QPixmap makeThumbnail(QPixmap image);
};

#endif // TOOLSMANAGER_H
