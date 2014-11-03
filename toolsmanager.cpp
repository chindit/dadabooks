#include "toolsmanager.h"

ToolsManager::ToolsManager(){
}

QString ToolsManager::downloadFile(QString file, QDir dossier){

    if(!dossier.exists()){
        dossier.mkdir(dossier.path());
    }

    //Et on télécharge les images

    QPixmap image;
    ToolsManager instance;
    image.loadFromData(instance.downloadLink(file));

    image = instance.makeThumbnail(image);

    QDateTime timestamp; timestamp = QDateTime::currentDateTime();
    QString nomImage = dossier.path()+"/"+QString::number(timestamp.toTime_t())+".png";
    image.save(nomImage);

    return nomImage;
}

QPixmap ToolsManager::getPixmapFromString(QString adresse){
    QPixmap pixmap;
    ToolsManager instance;
    if(!adresse.startsWith("http")){
        QFile fichierImage(adresse);
        if(!fichierImage.exists() && !adresse.isEmpty()){
            QMessageBox::information(0, "Image introuvable", "Une erreur est survenue, la jaquette de ce livre ne peut être trouvée");
        }
        else{
            pixmap.load(adresse);
        }
    }
    else{
        pixmap.loadFromData(instance.downloadLink(adresse));
    }
    pixmap = instance.makeThumbnail(pixmap);

    return pixmap;
}

QByteArray ToolsManager::downloadLink(QString url){
    QNetworkAccessManager nw_manager;
    QNetworkRequest request(url);
    QNetworkReply *reponse = nw_manager.get(request);
    QEventLoop eventLoop;
    QObject::connect(reponse, SIGNAL(finished()), &eventLoop, SLOT(quit()));
    eventLoop.exec();

    return reponse->readAll();
}

QPixmap ToolsManager::makeThumbnail(QPixmap image){
    QPixmap pixmap = image;
    int width = pixmap.width();
    if(width > 150){
        float coef = (float)width / 150;
        int result = pixmap.width()/coef;
        pixmap = pixmap.scaledToWidth(result);
    }

    return pixmap;
}
