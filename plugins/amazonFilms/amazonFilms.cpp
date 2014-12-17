#include "amazonFilms.h"
#include <cstdio>
#include <iostream>


QList< QMultiMap<QString, QString> > AmazonFilms::makeSearch(QString search){
    QString base_url = "http://www.amazon.fr/s/ref=nb_sb_noss/279-7665683-1736920?__mk_fr_FR=ÅMÅŽÕÑ&url=search-alias%3Ddvd&field-keywords=";
    base_url.append(search.replace(" ", "+"));
    QString contenu_page = this->download(base_url);

    contenu_page = contenu_page.left(contenu_page.indexOf("a-size-medium a-spacing-base a-color-base a-text-normal"));
    //Tableau des données
    QList< QMultiMap<QString, QString> > contenu;
    while(contenu_page.indexOf("a-section a-spacing-none a-inline-block s-position-relative", 0, Qt::CaseInsensitive) > 0){
        contenu_page = contenu_page.right(contenu_page.size()-contenu_page.indexOf("a-section a-spacing-none a-inline-block s-position-relative"));
        contenu_page = contenu_page.right(contenu_page.size()-14).trimmed();

        //ID
        QString livre_actuel2 = contenu_page.right(contenu_page.size()-contenu_page.indexOf("href=")-6);
        QString w_id = livre_actuel2.left(livre_actuel2.indexOf("\""));

        //Image
        livre_actuel2 = livre_actuel2.right(livre_actuel2.size()-livre_actuel2.indexOf("src=\""));
        QString w_image = livre_actuel2.right(livre_actuel2.size()-5);
        w_image = w_image.left(w_image.indexOf("\""));

        //Titre
        livre_actuel2 = livre_actuel2.right(livre_actuel2.size()-livre_actuel2.indexOf("a-size-base s-inline s-access-title a-text-normal"));
        livre_actuel2 = livre_actuel2.right(livre_actuel2.size()-livre_actuel2.indexOf(">")-1);
        QString w_titre = livre_actuel2.left(livre_actuel2.indexOf("<"));

        //Année
        QString w_annee = livre_actuel2.left(livre_actuel2.indexOf("</div"));
        QRegExp regAnnee("(\\d{4})");
        regAnnee.indexIn(w_annee);
        w_annee = regAnnee.capturedTexts().last();

        //Auteur
        livre_actuel2 = livre_actuel2.right(livre_actuel2.size()-livre_actuel2.indexOf("Distribution"));
        livre_actuel2 = livre_actuel2.right(livre_actuel2.size()-livre_actuel2.indexOf("\">")-2);
        QString w_acteurs = livre_actuel2.left(livre_actuel2.indexOf("<"));
        QTextDocument converter;
        converter.setHtml(w_acteurs);
        w_acteurs = converter.toPlainText();
        converter.clear();

        //Ajout dans la QMap
        QMultiMap <QString, QString> apercu_livre;
        apercu_livre.insert("image", w_image);
        apercu_livre.insert("id", w_id);
        apercu_livre.insert("titre", w_titre);
        apercu_livre.insert("auteur", w_acteurs);
        apercu_livre.insert("annee", w_annee);
        apercu_livre.insert("editeur", "");
        apercu_livre.insert("site", "Amazon");
        contenu.append(apercu_livre);
    }//Fin de la boucle
    return contenu;
}

QMultiMap<QString,QString> AmazonFilms::getBook( QString id ){
    QMultiMap<QString, QString> film;

    QString contenu_page = this->download(id);

    //Titre
    QString titre = contenu_page.right(contenu_page.size()-contenu_page.lastIndexOf("productTitle"));
    titre = titre.right(titre.size()-titre.indexOf(">")-1);
    titre = titre.left(titre.indexOf("<"));

    //Acteurs
    QString acteursTemp = contenu_page.right(contenu_page.size()-contenu_page.indexOf("<b>Acteurs", Qt::CaseSensitive)-10);
    acteursTemp = acteursTemp.left(acteursTemp.indexOf("</li>"));
    QStringList acteursList;
    while(acteursTemp.indexOf("<a", 0, Qt::CaseInsensitive) > 0){
        acteursTemp = acteursTemp.right(acteursTemp.size()-acteursTemp.indexOf("<a href")-4);
        acteursTemp = acteursTemp.right(acteursTemp.size()-acteursTemp.indexOf(">")-1);
        acteursList.append(acteursTemp.left(acteursTemp.indexOf("<")));
    }
    QString acteurs;
    foreach(QString item, acteursList){
        acteurs.append(item.trimmed());
        acteurs.append(",");
    }
    if(acteurs.endsWith(",")){
        acteurs.resize(acteurs.size()-1);
    }

    //Directeur
    QString directeurTemp = contenu_page.right(contenu_page.size()-contenu_page.indexOf("<b>Réalisateurs", Qt::CaseSensitive)-15);
    directeurTemp = directeurTemp.left(directeurTemp.indexOf("</li>"));
    QStringList directeursList;
    while(directeurTemp.indexOf("<a", 0, Qt::CaseInsensitive) > 0){
        directeurTemp = directeurTemp.right(directeurTemp.size()-directeurTemp.indexOf("<a href")-4);
        directeurTemp = directeurTemp.right(directeurTemp.size()-directeurTemp.indexOf(">")-1);
        directeursList.append(directeurTemp.left(directeurTemp.indexOf("<")));
    }
    QString directeurs;
    foreach(QString item, directeursList){
        directeurs.append(item.trimmed());
        directeurs.append(",");
    }
    if(directeurs.endsWith(",")){
        directeurs.resize(directeurs.size()-1);
    }

    //Année
    QString annee = contenu_page.right(contenu_page.size()-contenu_page.indexOf("Date de sortie"));
    annee = annee.right(annee.size()-annee.indexOf(">")-1);
    annee = annee.left(annee.indexOf("<"));
    QRegExp regAnnee("(\\d{4,})");
    regAnnee.indexIn(annee);
    annee = regAnnee.cap();

    //Durée
    QString duree = contenu_page.right(contenu_page.size()-contenu_page.indexOf("<b>Durée")-8);
    duree = duree.right(duree.size()-duree.indexOf(">")-1);
    duree = duree.left(duree.indexOf("<"));
    //On extrait juste le nombre de minutes
    QRegExp regDuree("(\\d{2,})");
    regDuree.indexIn(duree);
    duree = regDuree.cap();

    //Pays
    /*QString pays = contenu_page.right(contenu_page.size()-contenu_page.indexOf("País"));
    pays = pays.right(pays.size()-pays.indexOf("</span>")-7);
    pays = pays.left(pays.indexOf("</dd>"));
    if(pays.contains("<a href")){
        pays = pays.remove(0, pays.indexOf(">"));
        pays = pays.remove(pays.indexOf("<"), pays.size());
    }
    if(pays.startsWith("&nbsp;")){
        pays = pays.remove(0, pays.indexOf(";")+1);
    }*/

    //Genre
    /*QString genreTemp = contenu_page.right(contenu_page.size()-contenu_page.indexOf("<dt>Género"));
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
        genres.append(item);
        genres.append(",");
    }
    if(genres.endsWith(",")){
        genres.resize(genres.size()-1);
    }*/


    //Synopsis
    QString synopsis = contenu_page.right(contenu_page.size()-contenu_page.lastIndexOf("productDescriptionWrapper")-25);
    synopsis = synopsis.right(synopsis.size()-synopsis.indexOf("productDescriptionWrapper")-25);
    //synopsis = synopsis.right(synopsis.size()-synopsis.indexOf("<dd>")-4);
    synopsis = synopsis.left(synopsis.indexOf("%20%20%20%20", 25));
    //D'abord, on décode les %20
    synopsis = QUrl::fromPercentEncoding(synopsis.toUtf8());
    //Ensuite on décode le HTML
    QTextDocument converter;
    converter.setHtml(synopsis);
    synopsis = converter.toPlainText();
    converter.clear();
    if(synopsis.indexOf(">") >= 0 && synopsis.indexOf(">") < 15){
        synopsis = synopsis.right(synopsis.size()-synopsis.indexOf(">"));
    }


    //Image
    QString image = contenu_page.right(contenu_page.size()-contenu_page.indexOf("\"main\"")-6);
    image = image.right(image.size()-image.indexOf("\"")-1);
    image = image.left(image.indexOf("\""));

    film.insert("titre", titre.trimmed());
    film.insert("titreOriginal", "");
    film.insert("annee", annee);
    film.insert("duree", duree);
    film.insert("pays", "");
    film.insert("directeur", directeurs.trimmed());
    film.insert("acteurs", acteurs.trimmed());
    film.insert("genre", "");
    film.insert("synopsis", synopsis.trimmed());
    film.insert("langue", "Français");
    film.insert("couverture", image);
    return film;
}

QString AmazonFilms::getName(){
    QString nom = "Amazon";
    return nom;
}

QString AmazonFilms::getType(){
    QString type = "films";
    return type;
}

QString AmazonFilms::getLanguage(){
    return "FR";
}

#if QT_VERSION < 0x050000
Q_EXPORT_PLUGIN2(AmazonFilms, AmazonFilms)
#endif // QT_VERSION < 0x050000

QString AmazonFilms::download(QString url){
    QNetworkAccessManager nw_manager;
    QNetworkRequest request(url);
    QNetworkReply *reponse = nw_manager.get(request);
    QEventLoop eventLoop;
    QObject::connect(reponse, SIGNAL(finished()), &eventLoop, SLOT(quit()));
    eventLoop.exec();
    QByteArray data = reponse->readAll();
    QString contenu_page(data);

    return contenu_page;
}

