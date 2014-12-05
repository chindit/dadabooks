#ifndef TOOLSMANAGER_H
#define TOOLSMANAGER_H

#include <cmath>
#include <QDir>
#include <QEventLoop>
#include <QFileDialog>
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
    static void exportMovieList(QList<QMultiMap<QString, QString> > base, QString output, bool films, bool pdf=false);
    static QString guillemets(QString input);
    static QString stripSlashes(QString input);
    static QString raccourci(QString data, int longueur=50);

private:
    QByteArray downloadLink(QString url);
    QPixmap makeThumbnail(QPixmap image);
};

#endif // TOOLSMANAGER_H
