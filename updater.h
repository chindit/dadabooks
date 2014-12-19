#ifndef UPDATER_H
#define UPDATER_H

#include <QObject>
#include <QPushButton>
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

private:
    bool hasNewVersion();
    QString getNewVersion();

    QString version;
};

#endif // UPDATER_H
