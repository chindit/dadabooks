#include "allocine.h"


QList< QMultiMap<QString,QString> > Allocine::makeSearch(QString search){
    QString base_url = "http://www.allocine.fr/recherche/1/?q=";
    base_url.append(search.replace(" ", "+"));
    QString contenu_page = this->download(base_url);

    //Tableau des données
    QList< QMultiMap<QString, QString> > contenu;

    //Index des résultats
    contenu_page = contenu_page.mid(contenu_page.indexOf("<table"), contenu_page.indexOf("</table"));

    while(contenu_page.indexOf("<tr", 0, Qt::CaseInsensitive) > 0){
        //Image
        QString currentMovie = contenu_page.right(contenu_page.size()-contenu_page.indexOf("src='"));
        QString w_image = currentMovie.right(currentMovie.size()-5);
        w_image = w_image.left(w_image.indexOf("'"));

        //ID
        currentMovie = currentMovie.right(currentMovie.size()-currentMovie.indexOf("href=")-6);
        QString w_id = currentMovie.left(currentMovie.indexOf("'"));
        if(!w_id.contains("film"))
            break;
        if(!w_id.startsWith("http")){
            w_id.prepend("http://www.allocine.fr");
        }

        //Titre
        currentMovie = currentMovie.right(currentMovie.size()-currentMovie.indexOf(">")-1);
        QString w_titre = currentMovie.left(currentMovie.indexOf("</a"));
        w_titre =  w_titre.remove(QRegExp("<[^>]*>"));
        w_titre = w_titre.trimmed();

        //Année
        currentMovie = currentMovie.right(currentMovie.size()-currentMovie.indexOf("fs11\">")-6);
        QString w_annee = currentMovie.left(currentMovie.indexOf("<"));
        w_annee = w_annee.trimmed();

        //Directeur
        currentMovie = currentMovie.right(currentMovie.size()-currentMovie.indexOf(">")-1);
        QString w_directeur = currentMovie.left(currentMovie.indexOf("<")).trimmed();
        if(w_directeur.trimmed().startsWith("de"))
            w_directeur = w_directeur.right(w_directeur.size()-3);
        w_directeur = w_directeur.trimmed();

        //Acteurs
        //QString acteurs = currentMovie.right(currentMovie.size()-currentMovie.indexOf(">"));
        QString acteurs = currentMovie.mid(currentMovie.indexOf("<br />"), currentMovie.indexOf("<br"));
        acteurs = acteurs.right(acteurs.size()-6);
        if(acteurs.trimmed().startsWith("avec")){
            acteurs = acteurs.trimmed().right(acteurs.size()-5).trimmed();
        }

        contenu_page = currentMovie;

        //Ajout dans la QMap
        QMultiMap <QString, QString> apercu_livre;
        apercu_livre.insert("image", w_image);
        apercu_livre.insert("id", w_id);
        apercu_livre.insert("titre", w_titre);
        apercu_livre.insert("auteur", w_directeur);
        apercu_livre.insert("annee", w_annee);
        apercu_livre.insert("editeur", acteurs);
        apercu_livre.insert("site", "Allocine");
        contenu.append(apercu_livre);
    }//Fin de la boucle

    return contenu;
}

QMultiMap<QString,QString> Allocine::getBook(QString id){
    QMultiMap<QString, QString> film;
    QString contenu_page = this->download(id);

    //Image
    QString image = contenu_page.right(contenu_page.size()-contenu_page.indexOf("og:image\"")-9);
    image = image.right(image.size()-image.indexOf("\"")-1);
    image = image.left(image.indexOf("\""));

    //Titre
    QString titre = contenu_page.right(contenu_page.size()-contenu_page.indexOf("twitter:title\"")-14);
    titre = titre.right(titre.size()-titre.indexOf("\"")-1);
    titre = titre.left(titre.indexOf("\""));
    titre = this->convertUnicode(titre);


    //Titre original
    QString titreOriginal = ""; //Pas de titre original pour Allocine :(

    //Année
    QString annee = contenu_page.right(contenu_page.size()-contenu_page.indexOf("Date de sortie")-14);
    annee = annee.right(annee.size()-annee.indexOf("\">")-2);
    annee = annee.left(annee.indexOf("<"));
    QRegExp regAnnee("(\\d{4})");
    regAnnee.indexIn(annee);
    annee = regAnnee.cap();

    //Durée
    QString duree = contenu_page.right(contenu_page.size()-contenu_page.indexOf("Date de sortie")-14);
    duree = duree.right(duree.size()-duree.indexOf("(")-1);
    duree = duree.left(duree.indexOf(")"));
    if(duree.contains("h") || duree.contains("min"))
        duree = this->convertTime(duree);

    //Directeur
    QString directeur = contenu_page.right(contenu_page.size()-contenu_page.indexOf("video:director\"")-16);
    directeur = directeur.right(directeur.size()-directeur.indexOf("\"")-1);
    directeur = directeur.left(directeur.indexOf("\""));

    //Acteurs
    QString acteursTemp = contenu_page.right(contenu_page.size()-contenu_page.indexOf("Avec</span>")-11);
    acteursTemp = acteursTemp.left(acteursTemp.indexOf("</div>"));
    QStringList acteursList;
    while(acteursTemp.indexOf("<span") > 0){
        acteursTemp = acteursTemp.right(acteursTemp.size()-acteursTemp.indexOf("<span")-4);
        acteursTemp = acteursTemp.right(acteursTemp.size()-acteursTemp.indexOf('>')-1);
        QString actorName = (acteursTemp.left(acteursTemp.indexOf("<")));
        //Not selecting «plus» option
        if(actorName.trimmed() == "plus")
            continue;
        //Adding to main list
        acteursList.append(actorName.trimmed());
    }
    QString acteurs;
    for(QString actor : acteursList){
        acteurs.append(actor);
        acteurs.append(",");
    }
    //Removing trailing coma
    acteurs.remove(acteurs.size()-1, 1);


    //Genre
    QString genresTemp = contenu_page.right(contenu_page.size()-contenu_page.indexOf("Genres</span>")-13);
    genresTemp = genresTemp.left(genresTemp.indexOf("</div>"));
    QStringList genresList;
    while(genresTemp.indexOf("<span") > 0){
        genresTemp = genresTemp.right(genresTemp.size()-genresTemp.indexOf("itemprop")-4);
        genresTemp = genresTemp.right(genresTemp.size()-genresTemp.indexOf('>')-1);
        QString genreName = (genresTemp.left(genresTemp.indexOf("<")));
        //Not selecting «plus» option
        if(genreName.trimmed() == "plus")
            continue;
        //Adding to main list
        genresList.append(genreName.trimmed());
    }
    QString genres;
    for(QString type : genresList){
        genres.append(type);
        genres.append(",");
    }
    //Removing trailing coma
    genres.remove(genres.size()-1, 1);

    //Pays
    QString paysTemp = contenu_page.right(contenu_page.size()-contenu_page.indexOf("Nationalité</span>")-18);
    paysTemp = paysTemp.left(paysTemp.indexOf("</div>"));
    QStringList paysList;
    while(paysTemp.indexOf("<span") > 0){
        paysTemp = paysTemp.right(paysTemp.size()-paysTemp.indexOf("<span")-4);
        paysTemp = paysTemp.right(paysTemp.size()-paysTemp.indexOf('>')-1);
        QString paysName = (paysTemp.left(paysTemp.indexOf("<")));
        //Not selecting «plus» option
        if(paysName.trimmed() == "plus")
            continue;
        //Adding to main list
        paysList.append(paysName.trimmed());
    }
    QString pays;
    for(QString country : paysList){
        pays.append(country);
        pays.append(",");
    }
    //Removing trailing coma
    pays.remove(pays.size()-1, 1);

    //Synopsis
    QString synopsis = contenu_page.right(contenu_page.size()-contenu_page.indexOf("twitter:description\"")-20);
    synopsis = synopsis.right(synopsis.size()-synopsis.indexOf("\"")-1);
    synopsis = synopsis.left(synopsis.indexOf("\""));
    synopsis = this->convertUnicode(synopsis);

    film.insert("titre", titre.trimmed());
    film.insert("titreOriginal", titreOriginal.trimmed());
    film.insert("annee", annee);
    film.insert("duree", duree);
    film.insert("pays", pays);
    film.insert("directeur", directeur.trimmed());
    film.insert("acteurs", acteurs.trimmed());
    film.insert("genre", genres);
    film.insert("synopsis", synopsis.trimmed().remove(QRegExp("<[^>]*>")));
    film.insert("langue", "Français");
    film.insert("couverture", image);
    return film;
}

QString Allocine::getName(){
    QString nom = "Allocine";
    return nom;
}

QString Allocine::getType(){
    QString type = "films";
    return type;
}

QString Allocine::getLanguage(){
    return "FR";
}

QString Allocine::download(QString url){
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
            newUrl.prepend("http://www.allocine.com");
        }
        contenu_page = this->download(newUrl);
    }
    return contenu_page;
}

QString Allocine::convertTime(QString duree){
    QString copie = duree;
    int minutes = 0;
    if(duree.contains("h")){
        copie = duree.left(duree.indexOf("h"));
        int heures = copie.toInt();
        minutes = heures*60;
    }
    if(duree.contains("min")){
        copie = duree.right(duree.size()-duree.indexOf("h")-1);
        copie = copie.trimmed().left(copie.indexOf("m")-1);
        int minute = copie.toInt();
        minutes += minute;
    }
    return QString::number(minutes);
}

QString Allocine::convertUnicode(QString input){
    QTextDocument doc; doc.setHtml(input);
    input = doc.toPlainText();
    QRegExp regExp("(&#\\d+;)");
    QRegExp numbers("(\\d+)");
    regExp.indexIn(input);
    QStringList matches = regExp.capturedTexts();
    for(QString stringChar : matches){
        //Getting numbers
        numbers.indexIn(stringChar);
        QString capturedNumber = numbers.cap(); //Capturing numbers (ex: 039)
        QChar convertedUnicode(capturedNumber.toShort()); //Converting number into char
        input = input.replace(stringChar, QString(convertedUnicode)); //Replacing data
    }
    return input;
}

#if QT_VERSION < 0x050000
Q_EXPORT_PLUGIN2(Allocine, Allocine)
#endif // QT_VERSION < 0x050000
