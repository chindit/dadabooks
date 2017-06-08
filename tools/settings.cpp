#include "settings.h"

Settings::Settings()
{
    this->savedSettings = new QVariant[settingsNames.size()];

    this->loadSettings();
}

Settings::~Settings()
{
}

/**
 * Return required setting
 * @brief Settings::getSetting
 * @param setting
 * @return
 */
QVariant Settings::getSetting(Setting setting) {
    return savedSettings[setting];
}

/**
 * Save new setting if needed
 * @brief Settings::setSetting
 * @param s Setting
 * @param v QVariant
 */
void Settings::setSetting(Setting s, QVariant v){
    if(savedSettings[s] != v){
        savedSettings[s] = v;
        QSettings options("DadaBooks", "dadabooks");
        options.setValue(settingsNames.find(s)->second, savedSettings[s]);
    }
    else{
        return;
    }
}

QMap<QString, QVariant> Settings::getGroupSettings(QString groupName, QStringList settingNames)
{
    QSettings options("DadaBooks", "dadabooks");
    options.beginGroup(groupName);
    QMap<QString, QVariant> data;
    for (QString setting : settingNames) {
        data.insert(setting, options.value(setting, ""));
    }
    options.endGroup();
    return data;
}

void Settings::setGroupSettings(QString groupName, QMap<QString, QVariant> data)
{
    QSettings options("DadaBooks", "dadabooks");
    options.beginGroup(groupName);
    for (QMap<QString, QVariant>::iterator it = data.begin(); it != data.end(); ++it) {
        options.setValue(it.key(), it.value());
    }
    options.endGroup();
}

/**
 * Load current settings
 * @brief Settings::loadSettings
 */
void Settings::loadSettings() {
    QSettings options("DadaBooks", "dadabooks");

    // With «find» we get an iterator.  With «second» we take its value
    savedSettings[Setting::AutoAuteur] = options.value(settingsNames.find(AutoAuteur)->second, true);
    savedSettings[Setting::AutoEditeur] = options.value(settingsNames.find(AutoEditeur)->second, true);
    savedSettings[Setting::Sqlite] = options.value(settingsNames.find(Sqlite)->second, false);
    savedSettings[Setting::MariaDB] = options.value(settingsNames.find(MariaDB)->second, false);
    savedSettings[Setting::Xml] = options.value(settingsNames.find(Xml)->second, true);
    savedSettings[Setting::Fichier] = options.value(settingsNames.find(Fichier)->second, "");
    savedSettings[Setting::DBHost] = options.value(settingsNames.find(DBHost)->second, "localhost");
    savedSettings[Setting::DBUser] = options.value(settingsNames.find(DBUser)->second, "root");
    savedSettings[Setting::DBPass] = options.value(settingsNames.find(DBPass)->second, "");
    savedSettings[Setting::DBBase] = options.value(settingsNames.find(DBBase)->second, "DadaBooks");
    savedSettings[Setting::Initialized] = options.value(settingsNames.find(Initialized)->second, false);
    savedSettings[Setting::OpenInTab] = options.value(settingsNames.find(OpenInTab)->second, true);
    savedSettings[Setting::DownPics] = options.value(settingsNames.find(DownPics)->second, true);
    savedSettings[Setting::Type] = options.value(settingsNames.find(Type)->second, "livres");
    savedSettings[Setting::Empruntable] = options.value(settingsNames.find(Empruntable)->second, true);
    savedSettings[Setting::Verbosity] = options.value(settingsNames.find(Verbosity)->second, LogLevel::WARNING);
    savedSettings[Setting::StorageProvider] = options.value(settingsNames.find(StorageProvider)->second, "xml");
    return;
}

/**
 * Force settings to be reloaded from config file
 * @brief Settings::reload
 * @return
 */
bool Settings::reload()
{
    this->loadSettings();
    return true;
}
