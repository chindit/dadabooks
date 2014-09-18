#include "filmaffinity.h"


QList< QMultiMap<QString, QString> > Filmaffinity::makeSearch(QString search){
    QNetworkAccessManager nw_manager;
    QString base_url = "http://www.filmaffinity.com/es/search.php?stext=";
    base_url.append(search.replace(" ", "+"));
    QNetworkRequest request(base_url);
    QNetworkReply *reponse = nw_manager.get(request);
    QEventLoop eventLoop;
    QObject::connect(reponse, SIGNAL(finished()), &eventLoop, SLOT(quit()));
    eventLoop.exec();
    QByteArray data = reponse->readAll();
    QString contenu_page(data);

    //Tableau des données
    QList< QMultiMap<QString, QString> > contenu;

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
    film.insert("id", id);
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
