#include "settingsdialog.h"
#include "ui_settingsdialog.h"

SettingsDialog::SettingsDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SettingsDialog)
{
    ui->setupUi(this);
    insSettingsManager = new SettingsManager;
    this->prepareUi();

    QSignalMapper *map1, *map2, *map3;
    map1 = new QSignalMapper;
    map2 = new QSignalMapper;
    map3 = new QSignalMapper;
    connect(ui->groupBox_mysql, SIGNAL(clicked()), map1, SLOT(map()));
    map1->setMapping(ui->groupBox_mysql, 2);
    connect(map1, SIGNAL(mapped(int)), this, SLOT(manageCheck(int)));
    connect(ui->groupBox_sqlite, SIGNAL(clicked()), map2, SLOT(map()));
    map2->setMapping(ui->groupBox_sqlite, 1);
    connect(map2, SIGNAL(mapped(int)), this, SLOT(manageCheck(int)));
    connect(ui->groupBox_xml, SIGNAL(clicked()), map3, SLOT(map()));
    map3->setMapping(ui->groupBox_xml, 3);
    connect(map3, SIGNAL(mapped(int)), this, SLOT(manageCheck(int)));
}

SettingsDialog::~SettingsDialog()
{
    delete ui;
    delete insSettingsManager;
}

void SettingsDialog::accept(){
    //SQLite
    insSettingsManager->setSettings(Sqlite, ui->groupBox_sqlite->isChecked());
    insSettingsManager->setSettings(AutoAuteur, ui->checkBox_auteurs->isChecked());
    insSettingsManager->setSettings(AutoEditeur, ui->checkBox_editeurs->isChecked());
    //XML
    insSettingsManager->setSettings(Xml, ui->groupBox_xml->isChecked());
    //MariaDB
    insSettingsManager->setSettings(MariaDB, ui->groupBox_mysql->isChecked());
    insSettingsManager->setSettings(DBHost, ui->lineEdit_host->text());
    insSettingsManager->setSettings(DBUser, ui->lineEdit_user->text());
    insSettingsManager->setSettings(DBPass, ui->lineEdit_pass->text());
    insSettingsManager->setSettings(DBBase, ui->lineEdit_base->text());
    this->close();
}

void SettingsDialog::reset(){
    insSettingsManager->resetSettings();
    return;
}

void SettingsDialog::prepareUi(){
    //Update values
    //SQLite
    ui->groupBox_sqlite->setChecked(insSettingsManager->getSettings(Sqlite).toBool());
    ui->checkBox_auteurs->setChecked(insSettingsManager->getSettings(AutoAuteur).toBool());
    ui->checkBox_editeurs->setChecked(insSettingsManager->getSettings(AutoEditeur).toBool());
    //XML
    ui->groupBox_xml->setChecked(insSettingsManager->getSettings(Xml).toBool());
    //MySQL
    ui->groupBox_mysql->setChecked(insSettingsManager->getSettings(MariaDB).toBool());
    if(!insSettingsManager->getSettings(Xml).toBool())
        ui->horizontalLayout_3->setEnabled(false);
    return;
}

void SettingsDialog::manageCheck(int zone){
    if(zone == 1){
        ui->groupBox_mysql->setChecked(false);
        ui->groupBox_xml->setChecked(false);
        ui->horizontalLayout_3->setEnabled(false);
    }
    else if(zone == 2){
        ui->groupBox_sqlite->setChecked(false);
        ui->groupBox_xml->setChecked(false);
        ui->horizontalLayout_3->setEnabled(false);
    }
    else{
        ui->groupBox_sqlite->setChecked(false);
        ui->groupBox_mysql->setChecked(false);
        ui->horizontalLayout_3->setEnabled(true);
    }
    return;
}

void SettingsDialog::exportForGCStar(){
    XmlManager manager;
    manager.exportCurrentAsGCStar();
    return;
}
