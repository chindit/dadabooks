#ifndef TOOLSMANAGER_H
#define TOOLSMANAGER_H

#include <cmath>
#include <QtCore/QCryptographicHash>
#include <QtCore/QDir>
#include <QtCore/QEventLoop>
#include <QtWidgets/QFileDialog>
#include <QtGui/QIcon>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMessageBox>
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkRequest>
#include <QtNetwork/QNetworkReply>
#include <QtGui/QPainter>
#include <QtGui/QPixmap>
#include <QtWidgets/QProgressBar>
#include <QtPrintSupport/QPrinter>
#include <QtCore/QString>
#include <QtGui/QTextDocument>
#include <QtWidgets/QVBoxLayout>

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
