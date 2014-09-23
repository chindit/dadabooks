#include "filmaffinity.h"


QList< QMultiMap<QString, QString> > Filmaffinity::makeSearch(QString search){
    QString base_url = "http://www.filmaffinity.com/es/search.php?stext=";
    base_url.append(search.replace(" ", "+"));
    QString contenu_page = this->download(base_url);

    //Tableau des données
    QList< QMultiMap<QString, QString> > contenu;

    if(contenu_page.contains("Reparto") && contenu_page.contains("Director")){
        //On a trouvé directement le titre, on passe à l'édition
        QString url = contenu_page.right(contenu_page.size()-contenu_page.indexOf("og:url"));
        url = url.right(url.size()-url.indexOf("=\"")-2);
        url = url.left(url.indexOf("\""));
        QMultiMap<QString, QString> film;
        film.insert("id", url);
        //Titre
        QString titre = contenu_page.right(contenu_page.size()-contenu_page.indexOf("main-title"));
        titre = titre.right(titre.size()-titre.indexOf("name\">")-6);
        titre = titre.left(titre.indexOf("<"));

        //Année
        QString annee = contenu_page.right(contenu_page.size()-contenu_page.indexOf("Año"));
        annee = annee.right(annee.size()-annee.indexOf("<dd>")-4);
        annee = annee.left(annee.indexOf("</dd>"));

        //Directeur
        QString directeur = contenu_page.right(contenu_page.size()-contenu_page.indexOf("<dt>Director"));
        directeur = directeur.right(directeur.size()-directeur.indexOf("<dd>")-4);
        directeur = directeur.left(directeur.indexOf("</dd>"));
        if(directeur.contains("<a href")){
            directeur = directeur.remove(0, directeur.indexOf(">")+1);
            directeur = directeur.remove(directeur.indexOf("<"), directeur.size());
        }

        //Image
        QString image = contenu_page.right(contenu_page.size()-contenu_page.indexOf("og:image"));
        image = image.right(image.size()-image.indexOf("=\"")-2);
        image = image.left(image.indexOf("\""));

        //Acteurs
        QString acteursTemp = contenu_page.right(contenu_page.size()-contenu_page.indexOf("<dt>Reparto"));
        acteursTemp = acteursTemp.right(acteursTemp.size()-acteursTemp.indexOf("<dd>")-4);
        acteursTemp = acteursTemp.left(acteursTemp.indexOf("</dd>"));
        QStringList acteursList = acteursTemp.split(",");
        QString acteurs;
        foreach(QString item, acteursList){
            if(item.contains("<a href")){
                QString manoeuvre = item.remove(0, item.indexOf(">")+1);
                manoeuvre.resize(manoeuvre.indexOf("<"));
                acteurs.append(manoeuvre.trimmed());
                acteurs.append(", ");
            }
        }
        if(acteurs.endsWith(", ")){
            acteurs.resize(acteurs.size()-2);
        }

        film.insert("image", image);
        film.insert("titre", titre);
        film.insert("auteur", directeur);
        film.insert("editeur", acteurs);
        film.insert("annee", annee);
        contenu.append(film);

        return contenu;
    }

    while(contenu_page.indexOf("movie-card movie-card-1", 0, Qt::CaseInsensitive) > 0){
        contenu_page = contenu_page.right(contenu_page.size()-contenu_page.indexOf("movie-card movie-card-1"));
        contenu_page = contenu_page.right(contenu_page.size()-23).trimmed();

        //Image
        QString livre_actuel2 = contenu_page.right(contenu_page.size()-contenu_page.indexOf("src=\""));
        QString w_image = livre_actuel2.right(livre_actuel2.size()-5);
        w_image = w_image.left(w_image.indexOf("\""));

        //Titre
        livre_actuel2 = contenu_page.right(contenu_page.size()-contenu_page.indexOf("title"));
        livre_actuel2 = livre_actuel2.right(livre_actuel2.size()-livre_actuel2.indexOf("\"")-1);
        QString w_titre = livre_actuel2.left(livre_actuel2.indexOf("\""));

        //ID
        livre_actuel2 = livre_actuel2.right(livre_actuel2.size()-livre_actuel2.indexOf("href=")-6);
        QString w_id = livre_actuel2.left(livre_actuel2.indexOf("\""));
        if(!w_id.startsWith("http")){
            w_id.prepend("http://www.filmaffinity.com");
        }

        //Année
        livre_actuel2 = livre_actuel2.right(livre_actuel2.size()-livre_actuel2.indexOf("mc-title"));
        livre_actuel2 = livre_actuel2.right(livre_actuel2.size()-livre_actuel2.indexOf("</a>")-4);
        QString w_annee = livre_actuel2.left(livre_actuel2.indexOf("<"));
        w_annee = w_annee.trimmed();
        if(w_annee.startsWith("(")){
            w_annee.remove(0, 1);
        }
        if(w_annee.endsWith(")")){
            w_annee.resize(w_annee.size()-1);
        }

        //Auteur
        livre_actuel2 = livre_actuel2.right(livre_actuel2.size()-livre_actuel2.indexOf("mc-director\">")-13);
        livre_actuel2 = livre_actuel2.right(livre_actuel2.size()-livre_actuel2.indexOf("\">")-2);
        QString w_auteur = livre_actuel2.left(livre_actuel2.indexOf("<"));

        //Éditeur
        livre_actuel2 = livre_actuel2.right(livre_actuel2.size()-livre_actuel2.indexOf("mc-cast\">")-9);
        livre_actuel2 = livre_actuel2.right(livre_actuel2.size()-livre_actuel2.indexOf("\">")-2);
        QString w_editeur = livre_actuel2.left(livre_actuel2.indexOf("<"));
        w_editeur = w_editeur.right(w_editeur.size()-w_editeur.indexOf(":")-1).trimmed();
        if(w_editeur.endsWith(".")){
            w_editeur.resize(w_editeur.size()-1);
        }

        //Ajout dans la QMap
        QMultiMap <QString, QString> apercu_livre;
        apercu_livre.insert("image", w_image);
        apercu_livre.insert("id", w_id);
        apercu_livre.insert("titre", w_titre);
        apercu_livre.insert("auteur", w_auteur);
        apercu_livre.insert("annee", w_annee);
        apercu_livre.insert("editeur", w_editeur);
        contenu.append(apercu_livre);
    }//Fin de la boucle

    return contenu;
}

QMultiMap<QString,QString> Filmaffinity::getBook( QString id ){
    QMultiMap<QString, QString> film;

    QString contenu_page = this->download(id);

    //Titre
    QString titre = contenu_page.right(contenu_page.size()-contenu_page.indexOf("main-title"));
    titre = titre.right(titre.size()-titre.indexOf("name\">")-6);
    titre = titre.left(titre.indexOf("<"));

    //Titre original
    QString titreOriginal = contenu_page.right(contenu_page.size()-contenu_page.indexOf("Título original"));
    titreOriginal = titreOriginal.right(titreOriginal.size()-titreOriginal.indexOf("<dd>")-4);
    titreOriginal = titreOriginal.left(titreOriginal.indexOf("</dd>"));

    //Année
    QString annee = contenu_page.right(contenu_page.size()-contenu_page.indexOf("Año"));
    annee = annee.right(annee.size()-annee.indexOf("<dd>")-4);
    annee = annee.left(annee.indexOf("</dd>"));

    //Durée
    QString duree = contenu_page.right(contenu_page.size()-contenu_page.indexOf("Duración"));
    duree = duree.right(duree.size()-duree.indexOf("<dd>")-4);
    duree = duree.left(duree.indexOf("</dd>"));
    //On extrait juste le nombre de minutes
    QRegExp regDuree("(\\d{2,})");
    regDuree.indexIn(duree);
    duree = regDuree.cap();

    //Pays
    QString pays = contenu_page.right(contenu_page.size()-contenu_page.indexOf("País"));
    pays = pays.right(pays.size()-pays.indexOf("</span>")-7);
    pays = pays.left(pays.indexOf("</dd>"));
    if(pays.contains("<a href")){
        pays = pays.remove(0, pays.indexOf(">"));
        pays = pays.remove(pays.indexOf("<"), pays.size());
    }
    if(pays.startsWith("&nbsp;")){
        pays = pays.remove(0, pays.indexOf(";")+1);
    }

    //Directeur
    QString directeur = contenu_page.right(contenu_page.size()-contenu_page.indexOf("<dt>Director"));
    directeur = directeur.right(directeur.size()-directeur.indexOf("<dd>")-4);
    directeur = directeur.left(directeur.indexOf("</dd>"));
    if(directeur.contains("<a href")){
        directeur = directeur.remove(0, directeur.indexOf(">")+1);
        directeur = directeur.remove(directeur.indexOf("<"), directeur.size());
    }

    //Acteurs
    QString acteursTemp = contenu_page.right(contenu_page.size()-contenu_page.indexOf("<dt>Reparto"));
    acteursTemp = acteursTemp.right(acteursTemp.size()-acteursTemp.indexOf("<dd>")-4);
    acteursTemp = acteursTemp.left(acteursTemp.indexOf("</dd>"));
    QStringList acteursList = acteursTemp.split(",");
    QString acteurs;
    foreach(QString item, acteursList){
        if(item.contains("<a href")){
            QString manoeuvre = item.remove(0, item.indexOf(">")+1);
            manoeuvre.resize(manoeuvre.indexOf("<"));
            acteurs.append(manoeuvre.trimmed());
            acteurs.append(", ");
        }
    }
    if(acteurs.endsWith(", ")){
        acteurs.resize(acteurs.size()-2);
    }

    //Genre
    QString genreTemp = contenu_page.right(contenu_page.size()-contenu_page.indexOf("<dt>Género"));
    genreTemp = genreTemp.right(genreTemp.size()-genreTemp.indexOf("<dd>")-4);
    genreTemp = genreTemp.left(genreTemp.indexOf("</dd>"));
    QRegExp exp("\">(.*)</a>");
    exp.setMinimal(true);
    int position = genreTemp.indexOf(exp);
    QStringList genreList;
    while(position > -1){
        genreList.append(exp.cap(1));
        position = genreTemp.indexOf(exp, position+exp.cap(1).count());
    }
    QString genres;
    foreach(QString item, genreList){
        /*if(item.contains("<a href")){
            QString manoeuvre = item.remove(0, item.indexOf(">")+1);
            if(item.contains("<a href")){
                //Dans le cas où on a viré un <span> mais pas le lien
                manoeuvre = manoeuvre.remove(0, manoeuvre.indexOf(">")+1);
            }
            manoeuvre.resize(manoeuvre.indexOf("<"));
            genres.append(manoeuvre);
            genres.append(", ");
        }*/
        genres.append(item);
        genres.append(",");
    }
    if(genres.endsWith(",")){
        genres.resize(genres.size()-1);
    }


    //Synopsis
    QString synopsis = contenu_page.right(contenu_page.size()-contenu_page.indexOf("<dt>Sinopsis"));
    synopsis = synopsis.right(synopsis.size()-synopsis.indexOf("<dd>")-4);
    synopsis = synopsis.left(synopsis.indexOf("</dd>"));
    //On vire les parenthèses
    synopsis = synopsis.replace("([a-zA-Z0-9]{0,})", "");
    synopsis = synopsis.trimmed();

    //Image
    QString image = contenu_page.right(contenu_page.size()-contenu_page.indexOf("og:image"));
    image = image.right(image.size()-image.indexOf("=\"")-2);
    image = image.left(image.indexOf("\""));

    film.insert("titre", titre.trimmed());
    film.insert("titreOriginal", titreOriginal.trimmed());
    film.insert("annee", annee);
    film.insert("duree", duree);
    film.insert("pays", pays.trimmed());
    film.insert("directeur", directeur.trimmed());
    film.insert("acteurs", acteurs.trimmed());
    film.insert("genres", genres);
    film.insert("synopsis", synopsis.trimmed());
    film.insert("langue", "Castellano");
    film.insert("couverture", image);

    return film;
}

QString Filmaffinity::getName(){
    QString nom = "Filmaffinity";
    return nom;
}

QString Filmaffinity::getType(){
    QString type = "films";
    return type;
}

#if QT_VERSION < 0x050000
Q_EXPORT_PLUGIN2(Filmaffinity, Filmaffinity)
#endif // QT_VERSION < 0x050000

QString Filmaffinity::download(QString url){
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

