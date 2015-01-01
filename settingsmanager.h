#ifndef SETTINGSMANAGER_H
#define SETTINGSMANAGER_H

enum Setting { AutoAuteur, AutoEditeur, Sqlite, MariaDB, DBHost, DBUser, DBPass, DBBase, Xml, Fichier, Initialized, OpenInTab, DownPics, Type, Empruntable };

#include <QObject>
#include <QMessageBox>
#include <QVariant>
#include <QSettings>

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
