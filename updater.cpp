#include "updater.h"

Updater::Updater(QString version, QWidget *parent) : QWidget(parent){
    this->version = version;
}

Updater::~Updater(){

}

bool Updater::hasNewVersion(){
    QString currentVersion = ToolsManager::downloadLink("https://www.mailtunnel.eu/dadabooks/version.txt");
    return (currentVersion != this->version) ? true : false;
}

QString Updater::getNewVersion(){
    return QString();
}

void Updater::showUpdateDialog(){
    QDialog *dialogVersion = new QDialog();
    QGridLayout *layoutDialogVersion = new QGridLayout(dialogVersion);
    QLabel *titreDialog = new QLabel(tr("<h1>Mise à jour de DadaBooks</h1>"));
    QLabel *lineCurrentVersion = new QLabel(tr("<b>Votre version</b> : %s").arg(this->version));
    QLabel *lineNewVersion = new QLabel();
    QPushButton *pushButtonUpdate = new QPushButton(tr("Mettre à jour"));
    QPushButton *pushButtonClose = new QPushButton(tr("Fermer"));
    if(this->hasNewVersion())
        lineNewVersion->setText(tr("<b>Nouvelle version</b> : %s").arg(this->getNewVersion()));
    else{
        lineNewVersion->setText(tr("<b>Vous avez la dernière version! ;)</b>"));
        pushButtonUpdate->setEnabled(false);
    }
    layoutDialogVersion->addWidget(titreDialog, 0, 0, 1, 3, Qt::AlignHCenter);
    layoutDialogVersion->addWidget(lineCurrentVersion, 1, 0, 1, 3, Qt::AlignLeft);
    layoutDialogVersion->addWidget(lineNewVersion, 2, 0, 1, 3, Qt::AlignLeft);
    layoutDialogVersion->addWidget(pushButtonUpdate, 3, 1);
    layoutDialogVersion->addWidget(pushButtonClose, 3, 2);
    connect(pushButtonUpdate, SIGNAL(clicked()), this, SLOT(update()));
    connect(pushButtonClose, SIGNAL(clicked()), dialogVersion, SLOT(close()));
    dialogVersion->exec();
    delete dialogVersion;
    delete layoutDialogVersion;
    delete titreDialog;
    delete lineCurrentVersion;
    delete lineNewVersion;
    delete pushButtonClose;
    delete pushButtonUpdate;
    return;
}

