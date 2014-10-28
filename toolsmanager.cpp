#include "toolsmanager.h"

ToolsManager::ToolsManager(){
}

QString ToolsManager::downloadFile(QString file, QDir dossier){

    if(!dossier.exists()){
        dossier.mkdir(dossier.path());
    }

    //Et on télécharge les images
    QNetworkAccessManager nw_manager;
    QNetworkRequest request(file);
    QNetworkReply *reponse = nw_manager.get(request);
    QEventLoop eventLoop;
    QObject::connect(reponse, SIGNAL(finished()), &eventLoop, SLOT(quit()));
    eventLoop.exec();
    QPixmap image;
    image.loadFromData(reponse->readAll());
    int width = image.width();
    if(width > 150){
        float coef = (float)width / 150;
        int result = image.width()/coef;
        image = image.scaledToWidth(result);
    }
    QDateTime timestamp; timestamp = QDateTime::currentDateTime();
    QString nomImage = dossier.path()+"/"+QString::number(timestamp.toTime_t())+".png";
    image.save(nomImage);

    return nomImage;
}
