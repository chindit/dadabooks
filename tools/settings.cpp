#include "settings.h"

Settings::Settings()
{
    this->loadSettings();
    // TODO try this
    this->savedSettings = new QVariant[settingsNames.size()];
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
 * @param s
 * @param v
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

/**
 * Load current settings
 * @brief Settings::loadSettings
 */
void Settings::loadSettings() {
    QSettings options(QSettings::UserScope, "DadaBooks", "dadabooks");

    // With «find» we get an iterator.  With «second» we take its value
    QStringList test = options.allKeys();
    savedSettings[Setting::AutoAuteur] = options.value("test", true);
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
