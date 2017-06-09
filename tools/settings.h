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
    QMap<QString, QVariant> getGroupSettings(QString groupName, QStringList settingNames, bool isArray = false);
    void setGroupSettings(QString groupName, QMap<QString, QVariant> data, bool isArray = false);
    void setSetting(Setting, QVariant);
    bool reload();

private:
    void loadSettings();
    QVariant *savedSettings;
};

#endif // SETTINGS_H
