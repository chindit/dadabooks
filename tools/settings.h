#ifndef SETTINGS_H
#define SETTINGS_H

#include <QApplication>
#include <QSettings>

#include "config/config.h"

class Settings
{
public:
    Settings();
    ~Settings();
    QVariant getSetting(Setting);
    QMap<QString, QVariant> getGroupSettings(QString groupName, QStringList settingNames);
    void setGroupSettings(QString groupName, QMap<QString, QVariant> data);
    void setSetting(Setting, QVariant);
    bool reload();

private:
    void loadSettings();
    QVariant *savedSettings;
};

#endif // SETTINGS_H
