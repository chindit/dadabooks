#include "moviecovers.h"

QList< QMultiMap<QString,QString> > MovieCovers::makeSearch(QString search){
    QString base_url = "http://moviecovers.com/multicrit.html?titre=";
    base_url.append(search.replace(" ", "+"));
    QString contenu_page = this->download(base_url);

    //Tableau des données
    QList< QMultiMap<QString, QString> > contenu;

    //Index des résultats
    contenu_page = contenu_page.right(contenu_page.size()-contenu_page.indexOf("width=\"50%\""));

    while(contenu_page.indexOf("<a", 0, Qt::CaseInsensitive) > 0){
        //ID
        QString movieContent = contenu_page.right(contenu_page.size()-contenu_page.indexOf("href=")-6);
        contenu_page = movieContent; //Pour éviter les boucles infinies
        QString w_id = movieContent.left(movieContent.indexOf("\""));
        if(!w_id.contains("film"))
            break;
        if(!w_id.startsWith("http")){
            w_id.prepend("http://www.moviecovers.com");
        }

        //Titre
        movieContent = movieContent.right(movieContent.size()-movieContent.indexOf(">")-1);
        QString w_titre = movieContent.left(movieContent.indexOf("</"));
        w_titre = w_titre.trimmed();

        //Année
        movieContent = movieContent.right(movieContent.size()-movieContent.indexOf(">")-1);
        movieContent = movieContent.right(movieContent.size()-movieContent.indexOf("(")-1);
        QString w_annee = movieContent.left(movieContent.indexOf(")"));
        w_annee = w_annee.trimmed();

        //Ajout dans la QMap
        QMultiMap <QString, QString> apercu_livre;
        apercu_livre.insert("image", "");
        apercu_livre.insert("id", w_id);
        apercu_livre.insert("titre", w_titre);
        apercu_livre.insert("auteur", "");
        apercu_livre.insert("annee", w_annee);
        apercu_livre.insert("editeur", "");
        apercu_livre.insert("site", "Moviecovers");
        contenu.append(apercu_livre);
    }//Fin de la boucle

    return contenu;
}

QMultiMap<QString,QString> MovieCovers::getBook( QString id ){
    QMultiMap<QString, QString> film;

    QString contenu_page = this->download(id);

    //Titre
    QString titre = contenu_page.right(contenu_page.size()-contenu_page.indexOf("TITLE>"));
    titre = titre.right(titre.size()-titre.indexOf(">")-1);
    titre = this->ucFirst(titre.left(titre.indexOf("<")));

    //Titre original
    QString titreOriginal = contenu_page.right(contenu_page.size()-contenu_page.indexOf("Titre original"));
    titreOriginal = titreOriginal.right(titreOriginal.size()-titreOriginal.indexOf("\"top\">")-6);
    titreOriginal = titreOriginal.left(titreOriginal.indexOf("<"));
    titreOriginal = this->ucFirst(titreOriginal.simplified());

    //Image
    QString image = contenu_page.right(contenu_page.size()-contenu_page.indexOf("target=\"affiche\""));
    image = image.right(image.size()-image.indexOf("src=\"")-5);
    image = image.left(image.indexOf("\""));

    //Directeur
    QString directeur = contenu_page.right(contenu_page.size()-contenu_page.indexOf("alisateur"));
    directeur = directeur.right(directeur.size()-directeur.indexOf("href")-4);
    directeur = directeur.right(directeur.size()-directeur.indexOf(">")-1);
    directeur = directeur.left(directeur.indexOf("<"));

    //Année
    QString annee = contenu_page.right(contenu_page.size()-contenu_page.indexOf("Ann&eacute;e"));
    annee = annee.right(annee.size()-annee.indexOf("href")-4);
    annee = annee.right(annee.size()-annee.indexOf(">")-1);
    annee = annee.left(annee.indexOf("<"));

    //Pays
    QString paysFull = contenu_page.right(contenu_page.size()-contenu_page.indexOf("Nationalit"));
    paysFull = paysFull.right(paysFull.size()-paysFull.indexOf("\"top\">")-6);
    paysFull = paysFull.left(paysFull.indexOf("</TD>"));
    QString pays;
    while(paysFull.indexOf("href") > 0){
        paysFull = paysFull.right(paysFull.size()-paysFull.indexOf("href"));
        paysFull = paysFull.right(paysFull.size()-paysFull.indexOf(">")-1);
        QString resTemp = paysFull.left(paysFull.indexOf("<"));
        if(!resTemp.isEmpty()){
            pays.append(resTemp);
            pays.append(", ");
        }
    }
    if(pays.endsWith(", ")){
        pays = pays.left(pays.lastIndexOf(","));
    }
    //On converti les symboles HTML en texte normal
    QTextDocument converter;
    converter.setHtml(pays);
    pays = converter.toPlainText();
    converter.clear();

    //Genre
    QString genreFull = contenu_page.right(contenu_page.size()-contenu_page.indexOf("Genre"));
    genreFull = genreFull.right(genreFull.size()-genreFull.indexOf("\"top\">")-6);
    genreFull = genreFull.left(genreFull.indexOf("</TD>"));
    QString genres;
    while(genreFull.indexOf("href") > 0){
        genreFull = genreFull.right(genreFull.size()-genreFull.indexOf("href"));
        genreFull = genreFull.right(genreFull.size()-genreFull.indexOf(">")-1);
        QString resTemp = genreFull.left(genreFull.indexOf("<"));
        if(!resTemp.isEmpty()){
            genres.append(resTemp);
            genres.append(",");
        }
    }
    if(genres.endsWith(",")){
        genres = genres.left(genres.size()-1);
    }
    converter.setHtml(genres);
    genres = converter.toPlainText();
    converter.clear();

    //Durée
    QString duree = contenu_page.right(contenu_page.size()-contenu_page.indexOf("Dur&eacute;e"));
    duree = duree.right(duree.size()-duree.indexOf("href")-4);
    duree = duree.right(duree.size()-duree.indexOf(">")-1);
    duree = duree.left(duree.indexOf("<"));//ECHEC

    //Acteurs
    QString acteursFull = contenu_page.right(contenu_page.size()-contenu_page.indexOf("Acteurs principaux"));
    acteursFull = acteursFull.right(acteursFull.size()-acteursFull.indexOf("\"top\">")-6);
    acteursFull = acteursFull.left(acteursFull.indexOf("</TD>"));
    QString acteurs;
    while(acteursFull.indexOf("href") > 0){
        acteursFull = acteursFull.right(acteursFull.size()-acteursFull.indexOf("href"));
        acteursFull = acteursFull.right(acteursFull.size()-acteursFull.indexOf(">")-1);
        QString resTemp = acteursFull.left(acteursFull.indexOf("<"));
        if(!resTemp.isEmpty()){
            acteurs.append(resTemp);
            acteurs.append(",");
        }
    }
    if(acteurs.endsWith(",")){
        acteurs = acteurs.left(acteurs.size()-1);
    }

    //Synopsis
    QString synopsis = contenu_page.right(contenu_page.size()-contenu_page.indexOf("R&eacute;sum&eacute;"));
    synopsis = synopsis.right(synopsis.size()-synopsis.indexOf("justify\">")-9);
    synopsis = synopsis.left(synopsis.indexOf("<")-1);
    synopsis = synopsis.trimmed();
    converter.setHtml(synopsis);
    synopsis = converter.toPlainText();

    film.insert("titre", titre.trimmed());
    film.insert("titreOriginal", titreOriginal.trimmed());
    film.insert("annee", annee);
    film.insert("duree", duree);
    film.insert("pays", pays.trimmed());
    film.insert("directeur", directeur.trimmed());
    film.insert("acteurs", acteurs.trimmed());
    film.insert("genre", genres);
    film.insert("synopsis", synopsis.trimmed());
    film.insert("langue", "Français");
    film.insert("couverture", image);

    return film;
}

QString MovieCovers::getName(){
    QString nom = "Moviecovers";
    return nom;
}

QString MovieCovers::getType(){
    QString type = "films";
    return type;
}

QString MovieCovers::getLanguage(){
    return "FR";
}

QString MovieCovers::download(QString url){
    QNetworkAccessManager nw_manager;
    QNetworkRequest request(url);
    QNetworkReply *reponse = nw_manager.get(request);
    QEventLoop eventLoop;
    QObject::connect(reponse, SIGNAL(finished()), &eventLoop, SLOT(quit()));
    eventLoop.exec();
    QByteArray data = reponse->readAll();
    QString contenu_page(data);
    QVariant possibleRedirectUrl = reponse->attribute(QNetworkRequest::RedirectionTargetAttribute);
    if(!possibleRedirectUrl.toUrl().isEmpty()){
        QString newUrl = possibleRedirectUrl.toUrl().toString();
        if(!newUrl.startsWith("http")){
            newUrl.prepend("http://moviecovers.com");
        }
        contenu_page = this->download(newUrl);
    }
    return contenu_page;
}

QString MovieCovers::ucFirst(QString chaine){
    QString lowed = chaine.toLower().trimmed();
    QChar actual;
    actual = lowed.at(0).toUpper();
    lowed = lowed.replace(0, 1, lowed.at(0).toUpper());
    for(int i=0; i<lowed.size(); i++){
        if(lowed.at(i).isSpace())
            lowed = lowed.replace(i+1, 1, lowed.at(i+1).toUpper());
    }
    return lowed;
}

QString MovieCovers::convertTime(QString duree){
    QString copie = duree;
    int minutes = 0;
    if(duree.contains("h")){
        copie = duree.left(duree.indexOf("h"));
        int heures = copie.toInt();
        minutes = heures*60;
    }
    if(duree.contains("min")){
        copie = duree.right(duree.size()-duree.indexOf("h")-1);
        copie = copie.trimmed().left(copie.indexOf("m"));
        int minute = copie.toInt();
        minutes += minute;
    }
    return QString::number(minutes);
}

#if QT_VERSION < 0x050000
Q_EXPORT_PLUGIN2(moviecovers, MovieCovers)
#endif // QT_VERSION < 0x050000
