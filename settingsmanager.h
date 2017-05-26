#ifndef SETTINGSMANAGER_H
#define SETTINGSMANAGER_H

#include <QObject>
#include <QMessageBox>
#include <QVariant>
#include <QSettings>

#include "config/config.h"

class SettingsManager : public QObject
{
    Q_OBJECT
public:
    explicit SettingsManager(QObject *parent = 0);
    QVariant getSettings(Setting s);
    void setSettings(Setting s, QVariant v);
    
signals:
    
public slots:
    void loadSettings();
    void resetSettings();

private:
    QVariant *settings;
    QString *names;
    
};

#endif // SETTINGSMANAGER_H
