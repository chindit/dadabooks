#include "updater.h"

Updater::Updater(QString version, QWidget *parent) : QWidget(parent){
    this->version = version;
    download = new QWidget();
    bar = new QProgressBar();
}

Updater::~Updater(){
    delete download;
    delete bar;
}

bool Updater::hasNewVersion(){
    this->newVersion = ToolsManager::downloadLink("https://www.mailtunnel.eu/dadabooks/version.txt");
    this->newVersion = this->newVersion.trimmed();
    return (this->newVersion != this->version) ? true : false;
}

QString Updater::getNewVersion(){
    return this->newVersion;
}

void Updater::showUpdateDialog(){
    QDialog *dialogVersion = new QDialog(this);
    QGridLayout *layoutDialogVersion = new QGridLayout(dialogVersion);
    QLabel *titreDialog = new QLabel(tr("<h1>Mise à jour de DadaBooks</h1>"));
    QLabel *lineCurrentVersion = new QLabel(tr("<b>Votre version</b> : %1").arg(this->version));
    QLabel *lineNewVersion = new QLabel();
    QPushButton *pushButtonUpdate = new QPushButton(tr("Mettre à jour"));
    QPushButton *pushButtonClose = new QPushButton(tr("Fermer"));
    if(this->hasNewVersion())
        lineNewVersion->setText(tr("<b>Nouvelle version</b> : %1").arg(this->getNewVersion()));
    else{
        lineNewVersion->setText(tr("<b>Vous avez la dernière version! ;)</b>"));
        pushButtonUpdate->setEnabled(false);
    }
    layoutDialogVersion->addWidget(titreDialog, 0, 0, 1, 3, Qt::AlignHCenter);
    layoutDialogVersion->addWidget(lineCurrentVersion, 1, 0, 1, 3, Qt::AlignLeft);
    layoutDialogVersion->addWidget(lineNewVersion, 2, 0, 1, 3, Qt::AlignLeft);
    layoutDialogVersion->addWidget(pushButtonUpdate, 3, 1);
    layoutDialogVersion->addWidget(pushButtonClose, 3, 2);
    connect(pushButtonUpdate, SIGNAL(clicked()), this, SLOT(updateSoftware()));
    connect(pushButtonClose, SIGNAL(clicked()), dialogVersion, SLOT(close()));

    dialogVersion->show();
    return;
}

void Updater::updateSoftware(){
    QHBoxLayout *layout = new QHBoxLayout(this);
    layout->addWidget(bar);
    download->setLayout(layout);
    this->downloadFile();
    return;
}

void Updater::downloadFile(){
    download->show();

    reply = manager.get(QNetworkRequest(QUrl("https://www.mailtunnel.eu/iamabm24.7z.001")));

    connect(reply, SIGNAL(finished()), this, SLOT(finished()));
    connect(reply, SIGNAL(downloadProgress(qint64, qint64)), this, SLOT(updateBar(qint64, qint64)));
    return;
}

void Updater::updateBar(qint64 current, qint64 total){
    if (total != -1){
        bar->setRange(0, total);
        bar->setValue(current);
    }
    return;
}

void Updater::finished(){
    reply->deleteLater();
    QFile lastversion("application.exe");

    if(lastversion.open(QIODevice::WriteOnly)){
        lastversion.write(reply->readAll());
        lastversion.close();
        QMessageBox::information(this, "Fin de téléchargement", "Téléchargement terminé !");
    }
    download->close();
    return;
}

