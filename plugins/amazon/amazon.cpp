#include "amazon.h"
#include <iostream>

QList< QMultiMap<QString,QString> > Amazon::makeSearch(QString search){
    QNetworkAccessManager nw_manager;
    QString base_url = "http://www.amazon.fr/s/ref=nb_sb_noss?__mk_fr_FR=ÅMÅZÕÑ&url=search-alias%3Dstripbooks&field-keywords=";
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

    while(contenu_page.indexOf("imageContainer", 0, Qt::CaseInsensitive) > 0){
        contenu_page = contenu_page.right(contenu_page.size()-contenu_page.indexOf("imageContainer"));
        contenu_page = contenu_page.right(contenu_page.size()-14).trimmed();
        //Image
        QString livre_actuel2 = contenu_page.right(contenu_page.size()-contenu_page.indexOf("src=\""));
        QString w_image = livre_actuel2.right(livre_actuel2.size()-5);
        w_image = w_image.left(w_image.indexOf("\""));

        //ID
        livre_actuel2 = livre_actuel2.right(livre_actuel2.size()-livre_actuel2.indexOf("href=")-6);
        QString w_id = livre_actuel2.left(livre_actuel2.indexOf("\""));

        //Titre
        livre_actuel2 = livre_actuel2.right(livre_actuel2.size()-livre_actuel2.indexOf("bold\">")-6);
        QString w_titre = livre_actuel2.left(livre_actuel2.indexOf("<"));

        //Auteur
        livre_actuel2 = livre_actuel2.right(livre_actuel2.size()-livre_actuel2.indexOf("med reg\">"));
        livre_actuel2 = livre_actuel2.right(livre_actuel2.size()-livre_actuel2.indexOf(">")-1);
        if(livre_actuel2.trimmed().startsWith("de")){
            livre_actuel2 = livre_actuel2.right(livre_actuel2.size()-livre_actuel2.indexOf(">")-1);
        }
        QString w_auteur = livre_actuel2.left(livre_actuel2.indexOf("<"));

        //Année
        livre_actuel2 = livre_actuel2.right(livre_actuel2.size()-livre_actuel2.indexOf("(")-1);
        QString w_annee = livre_actuel2.left(livre_actuel2.indexOf(")"));
        w_annee = w_annee.split(" ").last();

        //Éditeur
        livre_actuel2 = livre_actuel2.right(livre_actuel2.size()-livre_actuel2.indexOf("lrg\">")-5);
        QString w_editeur = livre_actuel2.left(livre_actuel2.indexOf("<"));
        w_editeur = w_editeur.right(w_editeur.size()-w_editeur.indexOf(":")-1).trimmed();

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

QMultiMap<QString,QString> Amazon::getBook( QString id ){
    QNetworkAccessManager nw_manager;
    QNetworkRequest request(id);
    request.setRawHeader( "charset", "utf-8" );
    QNetworkReply *reponse = nw_manager.get(request);
    QEventLoop eventLoop;
    QObject::connect(reponse, SIGNAL(finished()), &eventLoop, SLOT(quit()));
    eventLoop.exec();
    QByteArray data = reponse->readAll();
    QString contenu_page(data);
    qDebug() << contenu_page;
    QString image = contenu_page.right(contenu_page.size()-contenu_page.indexOf("imageHashMain"));
    image = image.right(image.size()-image.indexOf("[")-1);
    image = image.left(image.indexOf("]"));
    image = image.split(",").last().trimmed();
    image = image.replace("\"", "");

    contenu_page = contenu_page.right(contenu_page.size()-contenu_page.indexOf("btAsinTitle"));
    contenu_page = contenu_page.right(contenu_page.size()-contenu_page.indexOf("0"));
    QString titre = contenu_page.right(contenu_page.size()-contenu_page.indexOf(">")-1);
    titre = titre.left(titre.indexOf("<")).trimmed();

    QString auteur = contenu_page.right(contenu_page.size()-contenu_page.indexOf("a href"));
    auteur = auteur.right(auteur.size()-auteur.indexOf(">")-1);
    auteur = auteur.left(auteur.indexOf("<")).trimmed();
    //ÉCHEC -> CHAÎNE VIDE

    QString editeur = contenu_page.right(contenu_page.size()-contenu_page.indexOf("Editeur"));
    editeur = editeur.right(editeur.size()-editeur.indexOf(">")-1);
    editeur = editeur.left(editeur.indexOf("<")).trimmed();

    QString annee = editeur.split("(").last();
    editeur = editeur.split("(").first().trimmed();
    QRegExp regAnnee("[0-9]{4}");
    annee = annee.right(annee.size()-regAnnee.indexIn(annee)).trimmed();
    if(annee.endsWith(")")){
        annee.resize(annee.size()-1);
        annee = annee.trimmed();
    }

    QTextDocument document;
    QString langue = contenu_page.right(contenu_page.size()-contenu_page.indexOf("Langue"));
    langue = langue.right(langue.size()-langue.indexOf(">")-1);
    langue = langue.left(langue.indexOf("<")).trimmed();
    document.setHtml(langue);
    langue = document.toPlainText();
    document.clear();
    //CARACTERES SPECIAUX

    QString isbn = contenu_page.right(contenu_page.size()-contenu_page.indexOf("ISBN-13"));
    isbn = isbn.right(isbn.size()-isbn.indexOf(">")-1);
    isbn = isbn.left(isbn.indexOf("<")).trimmed();

    QString pages = contenu_page.left(contenu_page.indexOf("pages<")+5).trimmed();
    pages = pages.right(30).trimmed();
    QStringList dataPages = pages.split(" ");
    for(int i=0; i<dataPages.size();i++){
        bool ok = false;
        dataPages.at(i).toInt(&ok);
        if(ok){
            pages = dataPages.at(i);
            break;
        }
    }

    QString resume = contenu_page.right(contenu_page.size()-contenu_page.indexOf("productDescriptionWrapper"));
    resume = resume.right(resume.size()-resume.indexOf(">")-1);
    resume = resume.left(resume.indexOf("<")).trimmed();
    document.setHtml(resume);
    QString resume2 = document.toPlainText();
    //CARACTÈRES SPECIAUX

    std::cout << pages.toStdString() << std::endl;
    std::cout << resume.toStdString() << std::endl;
    QMultiMap<QString, QString> livre;
    livre.insert("couverture", image);
    livre.insert("titre", titre);
    livre.insert("auteur", auteur);
    livre.insert("editeur", editeur);
    livre.insert("annee", annee);
    livre.insert("isbn", isbn);
    livre.insert("langue", langue);
    livre.insert("pages", pages);
    livre.insert("synopsis", resume2);
    livre.insert("xml", QString::number(1));

    return livre;
}

QString Amazon::getName(){
    QString nom = "Amazon.com";
    return nom;
}
