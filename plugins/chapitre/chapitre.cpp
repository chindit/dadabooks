#include "chapitre.h"

QList< QMultiMap<QString,QString> > Chapitre::makeSearch(QString search){
    QNetworkAccessManager nw_manager;
    QString base_url = "http://www.chapitre.com/CHAPITRE/fr/search/Default.aspx?optSearch=BOOKS&quicksearch=";
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

    while(contenu_page.indexOf("resultsProduct\">", 0, Qt::CaseInsensitive) > 0){
        contenu_page = contenu_page.right(contenu_page.size()-contenu_page.indexOf("resultsProduct\">"));
        contenu_page = contenu_page.right(contenu_page.size()-16).trimmed();

        //Image
        QString livre_actuel2 = contenu_page.right(contenu_page.size()-contenu_page.indexOf("src=\""));
        QString w_image = livre_actuel2.right(livre_actuel2.size()-5);
        w_image = w_image.left(w_image.indexOf("\""));

        //ID
        livre_actuel2 = livre_actuel2.right(livre_actuel2.size()-livre_actuel2.indexOf("href=")-6);
        QString w_id = livre_actuel2.left(livre_actuel2.indexOf("\""));
        if(!w_id.startsWith("http")){
            w_id.prepend("http://www.chapitre.com");
        }

        //Titre
        livre_actuel2 = livre_actuel2.right(livre_actuel2.size()-livre_actuel2.indexOf(">")-1);
        QString w_titre = livre_actuel2.left(livre_actuel2.indexOf("<"));

        //Auteur
        livre_actuel2 = livre_actuel2.right(livre_actuel2.size()-livre_actuel2.indexOf("href="));
        livre_actuel2 = livre_actuel2.right(livre_actuel2.size()-livre_actuel2.indexOf(">")-1);
        QString w_auteur = livre_actuel2.left(livre_actuel2.indexOf("<"));

        //Année
        livre_actuel2 = livre_actuel2.right(livre_actuel2.size()-livre_actuel2.indexOf("dateParution")-14);
        QString w_annee = livre_actuel2.left(livre_actuel2.indexOf("<"));
        w_annee = w_annee.right(w_annee.size()-w_annee.indexOf(":")).trimmed();
        if(w_annee.contains("/")){
            w_annee = w_annee.right(w_annee.size()-w_annee.lastIndexOf("/")).trimmed();
        }
        if(w_annee.endsWith(".")){
            w_annee.resize(w_annee.size()-1);
        }
        if(w_annee.startsWith(":")){
            w_annee = w_annee.right(w_annee.size()-1).trimmed();
        }
        if(w_annee.startsWith("/")){
            w_annee = w_annee.right(w_annee.size()-1).trimmed();
        }

        //Éditeur
        livre_actuel2 = livre_actuel2.right(livre_actuel2.size()-livre_actuel2.indexOf("editeur\">")-9);
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

QMultiMap<QString,QString> Chapitre::getBook( QString id ){
    QNetworkAccessManager nw_manager;
    QNetworkRequest request(id);
    QNetworkReply *reponse = nw_manager.get(request);
    QEventLoop eventLoop;
    QObject::connect(reponse, SIGNAL(finished()), &eventLoop, SLOT(quit()));
    eventLoop.exec();
    QByteArray data = reponse->readAll();
    QString contenu_page(data);

    QString image = contenu_page.right(contenu_page.size()-contenu_page.indexOf("itemprop=\"image\""));
    image = image.right(image.size()-image.indexOf("src=")-5);
    image = image.left(image.indexOf("\""));

    contenu_page = contenu_page.right(contenu_page.size()-contenu_page.indexOf("itemprop=\"name\""));
    QString titre = contenu_page.right(contenu_page.size()-contenu_page.indexOf(">")-1);
    titre = titre.left(titre.indexOf("<")).trimmed();

    QString auteur = contenu_page.right(contenu_page.size()-contenu_page.indexOf("itemprop=\"author\""));
    auteur = auteur.right(auteur.size()-auteur.indexOf(">")-1);
    auteur = auteur.left(auteur.indexOf("<")).trimmed();

    QString editeur = contenu_page.right(contenu_page.size()-contenu_page.indexOf("itemprop=\"publisher"));
    editeur = editeur.right(editeur.size()-editeur.indexOf(">")-1);
    editeur = editeur.left(editeur.indexOf("<")).trimmed();

    QString annee = contenu_page.right(contenu_page.size()-contenu_page.indexOf("itemprop=\"datePublished\""));
    annee = annee.right(annee.size()-annee.indexOf(">")-1);
    annee = annee.right(annee.size()-annee.indexOf("<span id"));
    annee = annee.right(annee.size()-annee.indexOf(">")-1);
    annee = annee.left(annee.indexOf("<"));
    if(annee.contains("/")){
        annee = annee.right(annee.size()-annee.lastIndexOf("/")).trimmed();
    }
    if(annee.endsWith(".")){
        annee.resize(annee.size()-1);
    }
    if(annee.startsWith(":")){
        annee = annee.right(annee.size()-1).trimmed();
    }
    if(annee.startsWith("/")){
        annee = annee.right(annee.size()-1).trimmed();
    }

    QString isbn = contenu_page.right(contenu_page.size()-contenu_page.indexOf("itemprop=\"isbn"));
    isbn = isbn.right(isbn.size()-isbn.indexOf(">")-1);
    isbn = isbn.left(isbn.indexOf("<")).trimmed();

    QString langue = contenu_page.right(contenu_page.size()-contenu_page.indexOf("itemprop=\"inLanguage\""));
    langue = langue.right(langue.size()-langue.indexOf(">")-1);
    langue = langue.left(langue.indexOf("<")).trimmed();

    QString pages = contenu_page.right(contenu_page.size()-contenu_page.indexOf("itemprop=\"numberOfPages"));
    if(contenu_page.indexOf("itemprop=\"numberOfPages") != -1){
        pages = pages.split(">").at(1);
        pages = pages.split("<").first();qDebug() << pages;

    }
    else
        pages = "";

    QString resume = contenu_page.right(contenu_page.size()-contenu_page.indexOf("itemprop=\"description\""));
    resume = resume.right(resume.size()-resume.indexOf("an>")-3);
    resume = resume.replace("<br>", "");
    resume = resume.replace("<br />", "");
    resume = resume.left(resume.indexOf("<")).trimmed();
    if(contenu_page.indexOf("class=\"cutedText\"") != -1){
        QString resume2 = contenu_page.right(contenu_page.size()-contenu_page.indexOf("class=\"cutedText\"")-18);
        resume2 = resume2.left(resume2.indexOf("<")).trimmed();
        resume = resume+" "+resume2;
    }
    resume = resume.trimmed();

    QMultiMap<QString, QString> livre;
    livre.insert("couverture", image);
    livre.insert("titre", titre);
    livre.insert("auteur", auteur);
    livre.insert("editeur", editeur);
    livre.insert("annee", annee);
    livre.insert("isbn", isbn);
    livre.insert("langue", langue);
    livre.insert("pages", pages);
    livre.insert("synopsis", resume);
    //livre.insert("xml", QString::number(1));

    return livre;
}

QString Chapitre::getName(){
    QString nom = "Chapitre.com";
    return nom;
}

QString Chapitre::getType(){
    QString type = "livres";
    return type;
}

//#if QT_VERSION < 0x050000
//Q_EXPORT_PLUGIN2(chapitre, Chapitre)
//#endif // QT_VERSION < 0x050000
