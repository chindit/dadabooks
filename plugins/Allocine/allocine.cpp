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
    QString image = contenu_page.right(contenu_page.size()-contenu_page.indexOf("poster\""));
    image = image.right(image.size()-image.indexOf("src='")-5);
    image = image.left(image.indexOf("'"));

    //Titre
    QString titre = contenu_page.right(contenu_page.size()-contenu_page.indexOf("alt='")-5);
    titre = titre.left(titre.indexOf("'"));
    QTextDocument txtTitre;
    txtTitre.setHtml(titre);
    titre = txtTitre.toPlainText();

    //Titre original
    QString titreOriginal = ""; //Pas de titre original pour Allocine :(

    //Année
    QString annee = contenu_page.right(contenu_page.size()-contenu_page.indexOf("datePublished")-13);
    annee = annee.right(annee.size()-annee.indexOf(">")-1);
    annee = annee.left(annee.indexOf("<"));
    QRegExp regAnnee("(\\d{4})");
    regAnnee.indexIn(annee);
    annee = regAnnee.cap();

    //Durée
    QString duree = contenu_page.right(contenu_page.size()-contenu_page.indexOf("duration")-8);
    duree = duree.right(duree.size()-duree.indexOf(">")-1);
    duree = duree.left(duree.indexOf("<"));
    if(duree.contains("h") || duree.contains("min"))
        duree = this->convertTime(duree);

    //Directeur
    QString directeur = contenu_page.right(contenu_page.size()-contenu_page.indexOf("director"));
    directeur = directeur.right(directeur.size()-directeur.indexOf("name\">")-6);
    directeur = directeur.left(directeur.indexOf("<"));

    //Acteurs
    QString acteursTemp = contenu_page.right(contenu_page.size()-contenu_page.indexOf("actors"));
    acteursTemp = acteursTemp.right(acteursTemp.size()-acteursTemp.indexOf("name\">")-6);
    acteursTemp = acteursTemp.left(acteursTemp.indexOf("</td"));
    QString acteurs = acteursTemp.remove(QRegExp("<[^>]*>"));
    if(acteurs.trimmed().endsWith("plus"))
            acteurs = acteurs.trimmed().left(acteurs.size()-5).trimmed();

    //Genre
    QString genreTemp = contenu_page.right(contenu_page.size()-contenu_page.indexOf("Genre", contenu_page.indexOf("actors"))-5);
    genreTemp = genreTemp.left(genreTemp.indexOf("</td>"));
    QRegExp exp("\">(.*)</span>");
    exp.setMinimal(true);
    int position = genreTemp.indexOf(exp);
    QStringList genreList;
    while(position > -1){
        genreList.append(exp.cap(1));
        position = genreTemp.indexOf(exp, position+exp.cap(1).count());
    }
    QString genres;
    foreach(QString item, genreList){
        if(item.contains(">"))
            genres.append(item.right(item.size()-item.indexOf(">")-1));
        else
            genres.append(item);
        genres.append(",");
    }
    if(genres.endsWith(",")){
        genres.resize(genres.size()-1);
    }

    //Pays
    QString pays = contenu_page.right(contenu_page.size()-contenu_page.indexOf("Nationalité"));
    pays = pays.right(pays.size()-pays.indexOf("</span>")-7);
    pays = pays.right(pays.size()-pays.indexOf("\">")-2);
    pays = pays.left(pays.indexOf("<")).trimmed();

    //Synopsis
    QString synopsis = contenu_page.right(contenu_page.size()-contenu_page.indexOf("itemprop=\"description"));
    synopsis = synopsis.right(synopsis.size()-synopsis.indexOf(">")-1);
    synopsis = synopsis.left(synopsis.indexOf("</p>"));

    film.insert("titre", titre.trimmed());
    film.insert("titreOriginal", titreOriginal.trimmed());
    film.insert("annee", annee);
    film.insert("duree", duree);
    film.insert("pays", pays.trimmed());
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
            newUrl.prepend("http://www.filmaffinity.com");
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
        copie = copie.trimmed().left(copie.indexOf("m"));
        int minute = copie.toInt();
        minutes += minute;
    }
    return QString::number(minutes);
}

#if QT_VERSION < 0x050000
Q_EXPORT_PLUGIN2(Allocine, Allocine)
#endif // QT_VERSION < 0x050000
