#ifndef TOOLSMANAGER_H
#define TOOLSMANAGER_H

#include <QDir>
#include <QEventLoop>
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
};

#endif // TOOLSMANAGER_H
