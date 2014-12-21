#ifndef UPDATER_H
#define UPDATER_H

#include <QApplication>
#include <QObject>
#include <QProgressBar>
#include <QPushButton>
#include <QProcess>
#include <QWidget>

#include "toolsmanager.h"

class Updater : public QWidget
{
    Q_OBJECT
public:
    explicit Updater(QString version, QWidget *parent = 0);
    ~Updater();

signals:

public slots:
    void showUpdateDialog();

private slots:
    void updateSoftware();
    void updateBar(qint64 current, qint64 total);
    void finished();

private:
    bool hasNewVersion();
    void downloadFile();
    QString getNewVersion();

    QProgressBar *bar;
    QWidget *download;
    QNetworkReply *reply;
    QLabel *currentProgress;
    QLabel *currentSize;

    QString version;
    QString newVersion;
    QNetworkAccessManager manager;
};

#endif // UPDATER_H
