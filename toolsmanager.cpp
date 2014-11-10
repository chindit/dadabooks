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

void ToolsManager::exportMovieList(QList<QMultiMap<QString, QString> > base, QString dir){
    if(base.isEmpty())
        return;
    QString document;
    QFile schema("ressources/template_films.html");
    if(schema.exists()){
        if(schema.open(QFile::ReadOnly)){
            document = schema.readAll();
            schema.close();
        }
    }

    for(int i=0; i<base.count(); ++i){
        QString image = base.at(i).value("couverture");
        if(image.startsWith("http")){
            image = ToolsManager::downloadFile(base.at(i).value("couverture"), QDir(dir));
        }
        document.append("<div class=\"film\"> \n \
                <img src=\""+image+"\" /> \n \
                <span class=\"titre\">"+base.at(i).value("titre")+"</span> \n \
                <div class=\"prop\"> \n \
                <span class=\"duree\">"+base.at(i).value("duree")+" min</span> \n \
                <span class=\"genre\">"+base.at(i).value("genre")+"</span> \n \
                <span class=\"date\">"+base.at(i).value("date")+"</span> \n \
                </div> \n \
                <span class=\"acteurs\">"+base.at(i).value("acteurs")+"</span> \n \
                <span class=\"synopsis\">"+base.at(i).value("synopsis")+"</span> \n \
            </div>");
    }
    document.append("</body></html>");
    schema.setFileName(dir+"/test.html");
    schema.open(QFile::WriteOnly);
    QTextStream out(&schema);
    out << document;
    schema.close();
    //TEST POUR IMPRESSION PDF -> À RE-CODER
    /*QTextDocument doc;
    doc.setHtml(document);
    QPrinter imprimante;
    imprimante.setOutputFileName("/home/david/Documents/file.pdf");
    imprimante.setOutputFormat(QPrinter::PdfFormat);
    doc.print(&imprimante);*/
}
