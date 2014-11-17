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

void ToolsManager::exportMovieList(QList<QMultiMap<QString, QString> > base, QString output, bool films, bool pdf){
    if(base.isEmpty())
        return;
    if(films){
        if(!pdf){
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
                    image = ToolsManager::downloadFile(base.at(i).value("couverture"), QDir::temp());
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
            schema.setFileName(output);
            schema.open(QFile::WriteOnly);
            QTextStream out(&schema);
            out << document;
            schema.close();
        }
        else{//PDF
            QPainter page;
            QPrinter printer;
            int totalWidth = printer.width();
            QFont titre("Arial", 22, QFont::DemiBold);
            QFont italique("Arial", 10); italique.setItalic(true);
            QFont standard("Arial", 10);
            QFontMetrics metrics(titre);
            QFontMetrics metricsItalique(italique);
            printer.setOutputFormat(QPrinter::PdfFormat);
            printer.setOutputFileName(output);
            printer.setPaperSize(QPrinter::A4);
            printer.setPageMargins(75, 100, 75, 100, QPrinter::DevicePixel);
            int currentHeight = 0;
            int iPage = 0;
            int nbPages = 1;
            if (!page.begin(&printer))
                return;
            for(int i=0; i<base.count(); ++i){
                QString image = base.at(i).value("couverture");
                if(image.startsWith("http")){
                    image = ToolsManager::downloadFile(base.at(i).value("couverture"), QDir::temp());
                }
                if((iPage*230) > (printer.height()-200)){
                    iPage = 0;
                    currentHeight = 0;
                    page.drawText(printer.width(), printer.height()+60, QString::number(nbPages));
                    nbPages++;
                    printer.newPage();
                }
                //1)Insertion de l'image
                page.drawPixmap(0, (iPage*230), 150, 220, QPixmap(image));
                //2)Titre
                page.setFont(titre);
                QRect placeTitre = metrics.boundingRect(base.at(i).value("titre"));
                page.drawText(160, currentHeight, totalWidth-150-160, placeTitre.height(), Qt::TextDontClip, base.at(i).value("titre"));
                currentHeight += placeTitre.height();
                page.setFont(italique);
                QRect placeGenre = metricsItalique.boundingRect(base.at(i).value("duree"));
                page.drawText(160, currentHeight, totalWidth, placeGenre.height(), Qt::AlignJustify, base.at(i).value("duree")+"min");
                currentHeight += placeGenre.height();
                page.drawText(160, currentHeight-placeGenre.height(), totalWidth-150-160, placeGenre.height(), Qt::AlignRight, base.at(i).value("genre"));
                page.drawText(160, currentHeight, totalWidth, placeGenre.height(), Qt::TextDontClip, base.at(i).value("acteurs").left(base.at(i).value("acteurs").left(75).lastIndexOf(",")));
                page.setFont(standard);
                page.drawText(160, currentHeight+placeGenre.height(), (totalWidth-150-160), (((iPage+1)*230)-currentHeight), Qt::AlignJustify|Qt::TextDontClip|Qt::TextWordWrap, base.at(i).value("synopsis"));
                currentHeight = (iPage+1)*230;
                iPage++;
            }
            //Numéro de la dernière page
            page.drawText(printer.width(), printer.height()+60, QString::number(nbPages));
            page.end();
        }
    }
    else{//LIVRES
        if(!pdf){
            //HTML LIVRES
        }
        else{//PDF LIVRES
            QPainter page;
            QPrinter printer;
            printer.setOutputFormat(QPrinter::PdfFormat);
            printer.setOutputFileName(output);
            printer.setPaperSize(QPrinter::A4);
            printer.setPageMargins(75, 100, 75, 100, QPrinter::DevicePixel);
            QTextDocument pdf_txt;
            QString chaine;
            /*if (!page.begin(&printer))
                return;*/
            for(int i=0; i<base.count(); ++i){
                chaine.append(base.at(i).value("auteur").toUpper());
                chaine.append(", <i>");
                chaine.append(base.at(i).value("titre"));
                chaine.append("</i>, ");
                chaine.append(base.at(i).value("editeur"));
                chaine.append(", ");
                chaine.append(base.at(i).value("annee"));
                chaine.append(".<br />");
            }
            pdf_txt.setHtml(chaine);
            pdf_txt.print(&printer);
        }
    }
}

//Modifie le type de guillemets pour les rendres compatibles avec SQL
QString ToolsManager::guillemets(QString input){
    QString output = input;
    bool close = false;
    for(int i=0; i<input.size(); i++){
        if(output.at(i) == QChar('"')){
            if(close){
                output.replace(i, 1, "»");
                close = false;
            }
            else{
                output.replace(i, 1, "«");
                close = true;
            }
        }
    }
    output = output.replace('\'', "\\\'");
    return output;
}

//Retire les slashes pour affichage
QString ToolsManager::stripSlashes(QString input){
    QString output = input;
    return output.replace("\\\'", "\'");
}
