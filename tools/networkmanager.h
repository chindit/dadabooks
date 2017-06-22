#ifndef NETWORKMANAGER_H
#define NETWORKMANAGER_H

#include <QByteArray>
#include <QEventLoop>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QObject>
#include <QUrl>

#include "tools/logger.h"
#include "tools/stringtools.h"

class NetworkManager : public QObject
{
    Q_OBJECT
public:
    explicit NetworkManager(Logger *logger, QObject *parent = nullptr);
    ~NetworkManager();
    QByteArray downloadFile(QString url);
    void download(QString url);
    QByteArray getFile();
    QString getError();

signals:
    void downloadFinished();
    void downloadAborted();
    void downloadFailed();

public slots:

private slots:
    void handleResponseStatus();

private:
    void setConnectors();
    bool isURLValid(QString url);

    Logger *logger;
    QNetworkAccessManager *manager;
    QNetworkReply *reply;
    QNetworkRequest *request;
    QString error;
    bool isLoaded;
};

#endif // NETWORKMANAGER_H
