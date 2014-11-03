#ifndef TOOLSMANAGER_H
#define TOOLSMANAGER_H

#include <QDir>
#include <QEventLoop>
#include <QMessageBox>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QPixmap>
#include <QString>

class ToolsManager
{
public:
    ToolsManager();
    static QString downloadFile(QString file, QDir dossier);
    static QPixmap getPixmapFromString(QString adresse);

private:
    QByteArray downloadLink(QString url);
    QPixmap makeThumbnail(QPixmap image);
};

#endif // TOOLSMANAGER_H
