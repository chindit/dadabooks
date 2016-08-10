#ifndef TOOLSMANAGER_H
#define TOOLSMANAGER_H

#include <cmath>
#include <QCryptographicHash>
#include <QDir>
#include <QEventLoop>
#include <QFileDialog>
#include <QIcon>
#include <QLabel>
#include <QMessageBox>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QPainter>
#include <QPixmap>
#include <QProgressBar>
#include <QtPrintSupport/QPrinter>
#include <QString>
#include <QTextDocument>
#include <QVBoxLayout>

class ToolsManager
{
public:
    ToolsManager();
    static QString downloadFile(QString file, QDir dossier);
    static QPixmap getPixmapFromString(QString adresse);
    static void exportMovieList(QList<QMultiMap<QString, QString> > base, QString output, bool films, bool pdf=false, QList<int> ordre=QList<int>());
    static QString guillemets(QString input);
    static QString stripSlashes(QString input);
    static QString raccourci(QString data, int longueur=50);
    static QString stripDeterminants(QString titre);
    static QByteArray downloadLink(QString url);
    static QString splitWordString(QString chaine, int nbMots);

private:
    QPixmap makeThumbnail(QPixmap image);
};

#endif // TOOLSMANAGER_H
