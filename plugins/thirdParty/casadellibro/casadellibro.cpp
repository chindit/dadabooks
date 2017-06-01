#include "casadellibro.h"

QList< QMultiMap<QString,QString> > Casadellibro::makeSearch(QString search){
    QNetworkAccessManager nw_manager;
    QString base_url = "http://www.casadellibro.com/busqueda-libros?busqueda=";
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

    while(contenu_page.indexOf("mod-list-item\">", 0, Qt::CaseInsensitive) > 0){
        contenu_page = contenu_page.right(contenu_page.size()-contenu_page.indexOf("mod-list-item\">"));
        contenu_page = contenu_page.right(contenu_page.size()-16).trimmed();

        //ID
        QString w_id = contenu_page.right(contenu_page.size()-contenu_page.indexOf("title-link searchResult"));
        w_id = w_id.right(w_id.size()-w_id.indexOf("href=")-6);
        w_id = w_id.left(w_id.indexOf("\""));
        if(!w_id.startsWith("http")){
            w_id.prepend("http://www.casadellibro.com");
        }

        //Image
        contenu_page = contenu_page.right(contenu_page.size()-contenu_page.indexOf("src=\""));
        QString w_image = contenu_page.right(contenu_page.size()-5);
        w_image = w_image.left(w_image.indexOf("\""));

        //Titre
        contenu_page = contenu_page.right(contenu_page.size()-contenu_page.indexOf("title-link"));
        contenu_page = contenu_page.right(contenu_page.size()-contenu_page.indexOf(">")-1);
        QString w_titre = contenu_page.left(contenu_page.indexOf("<"));

        //Auteur
        contenu_page = contenu_page.right(contenu_page.size()-contenu_page.indexOf("author-link"));
        contenu_page = contenu_page.right(contenu_page.size()-contenu_page.indexOf(">")-1);
        QString w_auteur = contenu_page.left(contenu_page.indexOf("<"));

        //Éditeur
        contenu_page = contenu_page.right(contenu_page.size()-contenu_page.indexOf("mod-libros-editorial"));
        contenu_page = contenu_page.right(contenu_page.size()-contenu_page.indexOf(">")-1);
        QString w_editeur = contenu_page.left(contenu_page.indexOf("<"));
        if(w_editeur.trimmed().endsWith(","))
            w_editeur.resize(w_editeur.size()-1);
        w_editeur = w_editeur.trimmed();

        //Année
        contenu_page = contenu_page.right(contenu_page.size()-contenu_page.indexOf(">")-1);
        QString w_annee = contenu_page.left(contenu_page.indexOf("<"));
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

        //Ajout dans la QMap
        QMultiMap <QString, QString> apercu_livre;
        apercu_livre.insert("image", w_image);
        apercu_livre.insert("id", w_id);
        apercu_livre.insert("titre", w_titre);
        apercu_livre.insert("auteur", w_auteur);
        apercu_livre.insert("annee", w_annee);
        apercu_livre.insert("editeur", w_editeur);
        apercu_livre.insert("site", "Casa del libro");
        contenu.append(apercu_livre);
    }//Fin de la boucle

    return contenu;
}

QMultiMap<QString,QString> Casadellibro::getBook( QString id ){
    QNetworkAccessManager nw_manager;
    QNetworkRequest request(id);
    QNetworkReply *reponse = nw_manager.get(request);
    QEventLoop eventLoop;
    QObject::connect(reponse, SIGNAL(finished()), &eventLoop, SLOT(quit()));
    eventLoop.exec();
    QByteArray data = reponse->readAll();
    QString contenu_page(data);

    QString image = contenu_page.right(contenu_page.size()-contenu_page.indexOf("imgPrincipal"));
    image = image.right(image.size()-image.indexOf("src=")-5);
    image = image.left(image.indexOf("\""));

    contenu_page = contenu_page.right(contenu_page.size()-contenu_page.indexOf("book-header-2-title"));
    QString titre = contenu_page.right(contenu_page.size()-contenu_page.indexOf(">")-1);
    titre = titre.left(titre.indexOf("<")).trimmed();

    QString auteur = contenu_page.right(contenu_page.size()-contenu_page.indexOf("book-header-2-subtitle-author"));
    auteur = auteur.right(auteur.size()-auteur.indexOf(">")-1);
    auteur = auteur.left(auteur.indexOf("<")).trimmed();

    contenu_page = contenu_page.right(contenu_page.size()-contenu_page.indexOf("book-header-2-subtitle-publisher"));
    QString editeur = contenu_page.right(contenu_page.size()-contenu_page.indexOf(">")-1);
    editeur = editeur.left(editeur.indexOf("<")).trimmed();

    QString annee = contenu_page.right(contenu_page.size()-contenu_page.indexOf("an>")-3);
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
    if(annee.startsWith(",")){
        annee = annee.remove(0, 1);
        annee = annee.trimmed();
    }

    QString isbn = contenu_page.right(contenu_page.size()-contenu_page.indexOf("book-header-2-subtitle-isbn"));
    isbn = isbn.right(isbn.size()-isbn.indexOf(">")-1).trimmed();
    isbn = isbn.right(isbn.size()-isbn.indexOf(">")-1);
    isbn = isbn.left(isbn.indexOf("<")).trimmed();

    QString pages = contenu_page.right(contenu_page.size()-contenu_page.indexOf("ginas"));
    if(contenu_page.indexOf("ginas") != -1){
        pages = pages.right(pages.size()-pages.indexOf(">")-1);
        pages = pages.left(pages.indexOf("<")).trimmed();
        pages = pages.split("p").first().trimmed();
    }
    else
        pages = "";

    QString langue = contenu_page.right(contenu_page.size()-contenu_page.indexOf("Lengua:"));
    langue = langue.right(langue.size()-langue.indexOf(">")-1);
    langue = langue.left(langue.indexOf("<")).trimmed();

    QString resume = contenu_page.right(contenu_page.size()-contenu_page.lastIndexOf("Resumen del libro"));
    resume = resume.right(resume.size()-resume.indexOf("\">")-2);
    resume = resume.replace("<br>", "");
    resume = resume.replace("<br />", "");
    resume = resume.left(resume.indexOf("<")).trimmed();
    if(contenu_page.indexOf("expand-hide") != -1){
        QString resume2 = contenu_page.right(contenu_page.size()-contenu_page.indexOf("expand-hide")-13);
        resume2 = resume2.left(resume2.indexOf("<")).trimmed();
        resume = resume+" "+resume2;
    }
    resume = resume.trimmed();

    QMultiMap<QString, QString> livre;
    livre.insert("couverture", image);
    livre.insert("titre", this->ucFirst(titre));
    livre.insert("auteur", this->ucFirst(auteur));
    livre.insert("editeur", this->ucFirst(editeur));
    livre.insert("annee", annee);
    livre.insert("isbn", isbn);
    livre.insert("langue", this->ucFirst(langue));
    livre.insert("pages", pages);
    livre.insert("synopsis", resume);
    //livre.insert("xml", QString::number(1));

    return livre;
}

QString Casadellibro::getName(){
    QString nom = "Casa del libro";
    return nom;
}

QString Casadellibro::getType(){
    QString type = "livres";
    return type;
}

QString Casadellibro::getLanguage(){
    return "ES";
}

QString Casadellibro::ucFirst(QString chaine){
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

//#if QT_VERSION < 0x050000
//Q_EXPORT_PLUGIN2(casadellibro, CasaDelLibro)
//#endif // QT_VERSION < 0x050000

