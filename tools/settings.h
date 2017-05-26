#ifndef SETTINGS_H
#define SETTINGS_H

#include <QSettings>

#include "config/config.h"

class Settings
{
public:
    Settings();
    ~Settings();
    QVariant getSetting(Setting);
    void setSetting(Setting, QVariant);
    bool reload();

private:
    void loadSettings();
    QVariant *savedSettings;
};

#endif // SETTINGS_H
