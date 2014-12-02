#include "xmlmanager.h"
#include "sitemanager.h"

XmlManager::XmlManager(QObject *parent) : QObject(parent){
    insManager = new SettingsManager;
}

XmlManager::~XmlManager(){
    delete insManager;
}

QList< QMultiMap<QString,QString> > XmlManager::readBase(int id){
    QDomDocument doc;
    QList< QMultiMap<QString,QString> > donnees;
    QFile file(insManager->getSettings(Fichier).toString());
    if (!file.open(QIODevice::ReadOnly))
        return donnees;
    if (!doc.setContent(&file)){
        file.close();
        return donnees;
    }
    file.close();

    int i=0;
    QDomNodeList tab;
    QDomElement livre;
    QDomNode n;
    QDomElement racine = doc.documentElement();
    QDomNode noeud = racine.firstChild();
    while(!noeud.isNull()){
        livre = noeud.toElement();
        if(livre.tagName() == "livre" || livre.tagName() == "film"){
            QMultiMap<QString, QString> book;
            book.insert("id", livre.attribute("id"));
            tab = livre.childNodes();
            for(i=0;i<tab.length();i++){
                n = tab.item(i);
                book.insert(n.nodeName(), n.firstChild().toText().data());
            }
            if(id == 0 || book.value("id").toInt() == id){
                donnees.append(book);
            }
        }
        noeud = noeud.nextSibling();
    }
    return donnees;
}

QMultiMap<QString,QString> XmlManager::getBook(int id){
    QList< QMultiMap<QString,QString> > resultat = this->readBase(id);
    if(resultat.size() != 1){
        exit(EXIT_FAILURE);
    }
    else{
        return resultat.at(0);
    }
}

void XmlManager::deleteBook(int id){
    QList<QMultiMap<QString, QString> > base = this->readBase();
    for(int i=0; i<base.size(); i++){
        if(base.at(i).value("id").toInt() == id){
            base.removeAt(i);
            break;
        }
    }
    this->writeBase(base);
    return;
}

bool XmlManager::writeBase(QList<QMultiMap<QString, QString> > base){
    QDomDocument doc;
    QDomElement livres = doc.createElement((QString::compare(insManager->getSettings(Type).toString(), "films", Qt::CaseInsensitive) == 0) ? "films" : "livres");
    doc.appendChild(livres);

    for(int i=0; i<base.size(); i++){
        QMultiMap<QString, QString> livreActuel = base.at(i);
        QDomElement livre = doc.createElement((QString::compare(insManager->getSettings(Type).toString(), "films", Qt::CaseInsensitive) == 0) ? "film" : "livre");
        livres.appendChild(livre);
        livre.setAttribute("id", livreActuel.value("id"));

        //Iterator
        QMultiMap<QString, QString>::iterator it;
        for(it = livreActuel.begin(); it != livreActuel.end(); ++it){
            if(it.key() != "id"){
                QDomElement elem = doc.createElement(it.key());
                livre.appendChild(elem);
                QDomText textElem = doc.createTextNode(it.value());
                elem.appendChild(textElem);
            }
        }
    }

    QFile doc_xml(insManager->getSettings(Fichier).toString());
        if (!doc_xml.open(QIODevice::WriteOnly)){
            return false;
        }
        QTextStream sortie;
        sortie.setDevice(&doc_xml);
    doc.save(sortie, 4);
    doc_xml.close();
    return true;
}

void XmlManager::addBook(QMultiMap<QString, QString> livre){
    QList<QMultiMap<QString,QString> > base = this->readBase();
    int id_high = 0;
    if(livre.value("key", "-1").toInt() < 0){
        for(int i=0; i<base.size(); i++){
            if(base.at(i).value("id").toInt() > id_high)
                id_high = base.at(i).value("id").toInt();
        }
        livre.insert("id", QString::number(id_high+1));
    }
    base.append(livre);
    this->writeBase(base);
    return;
}

QList<QMultiMap<QString, QString> > XmlManager::makeSearch(QStringList mots){
    QList<QMultiMap<QString, QString> > base = this->readBase();
    QList<QMultiMap<QString, QString> > resultats;
    for(int i=0; i<base.size(); i++){
        QMultiMap<QString, QString> livreActuel = base.at(i);
        QMapIterator<QString, QString> it=(livreActuel);
        bool added = false;
        while(it.hasNext()){
            it.next();
            for(int j=0; j<mots.size(); j++){
                if(mots.at(j).size() > 3){
                    if(!added && it.value().contains(mots.at(j), Qt::CaseInsensitive)){
                        resultats.append(livreActuel);
                        added = true;
                    }
                }
            }
        }
    }

    return resultats;
}

int XmlManager::getIdByTitle(QString titre){
    QList<QMultiMap<QString, QString> > base = this->readBase();
    for(int i=0; i<base.size(); i++){
        if(base.at(i).value("titre") == titre){
            return base.at(i).value("id").toInt();
        }
    }
    //Si on est ici, c'est que la recherche a échoué
    return -1;
}

void XmlManager::exportCurrentAsGCStar(QList< QMultiMap<QString, QString> > base){
    QDomDocument doc;
    QDomElement collection = doc.createElement("collection");
    collection.setAttribute("type", "GCbooks");
    collection.setAttribute("items", base.size());
    collection.setAttribute("version", "1.7.0");
    doc.appendChild(collection);

    int id_high = 0;
    for(int i=0; i<base.size(); i++){
        if(base.at(i).value("id").toInt() > id_high)
            id_high = base.at(i).value("id").toInt();
    }

    QDomElement information = doc.createElement("information");
    QDomElement maxId = doc.createElement("maxId");
    QDomText textMaxId = doc.createTextNode(QString::number(id_high));
    maxId.appendChild(textMaxId);
    information.appendChild(maxId);
    collection.appendChild(information);

    for(int i=0; i<base.size(); i++){
        QDomElement item = doc.createElement("item");
        item.setAttribute("id", base.at(i).value("id"));
        item.setAttribute("title", base.at(i).value("titre"));
        item.setAttribute("cover", ""); //Non supporté pour l'instant
        item.setAttribute("backpic", ""); //??
        item.setAttribute("publisher", base.at(i).value("editeur"));
        item.setAttribute("publication", base.at(i).value("annee"));
        item.setAttribute("language", base.at(i).value("langue"));
        item.setAttribute("serie", "");
        item.setAttribute("rank", "1"); //??
        item.setAttribute("edition", base.at(i).value("edition"));
        item.setAttribute("format", "");
        item.setAttribute("pages", base.at(i).value("pages"));
        item.setAttribute("web", ""); //Non supporté -> id de la page
        item.setAttribute("read", base.at(i).value("lu").toInt());
        QDate date;
        item.setAttribute("added", date.toString("dd/MM/yyyy"));
        item.setAttribute("rating", base.at(i).value("note").toInt());
        item.setAttribute("acquisition", "");
        item.setAttribute("location", base.at(i).value("classement"));
        item.setAttribute("translator", "");
        item.setAttribute("artist", "");
        item.setAttribute("digitalfile", "");
        item.setAttribute("borrower", "none");
        item.setAttribute("lendDate", "");
        item.setAttribute("borrowings", "");
        item.setAttribute("favourite", "0");
        item.setAttribute("tags", "");
        collection.appendChild(item);
        QDomElement synopsis = doc.createElement("description");
        QDomText textSynopsis = doc.createTextNode(base.at(i).value("synopsis"));
        synopsis.appendChild(textSynopsis);
        item.appendChild(synopsis);
        QDomElement comments = doc.createElement("comments");
        QDomText textComments = doc.createTextNode(base.at(i).value("commentaire"));
        comments.appendChild(textComments);
        item.appendChild(comments);
        QDomElement authors = doc.createElement("authors");
        QDomElement line = doc.createElement("line");
        QDomElement col = doc.createElement("col");
        QDomText textCol = doc.createTextNode(base.at(i).value("auteur"));
        col.appendChild(textCol);
        line.appendChild(col);
        authors.appendChild(line);
        item.appendChild(authors);
        QDomElement genre = doc.createElement("genre");
        QDomElement lineGenre = doc.createElement("line");
        QDomElement colGenre = doc.createElement("col");
        QDomText textGenre = doc.createTextNode("Roman");
        colGenre.appendChild(textGenre);
        lineGenre.appendChild(colGenre);
        genre.appendChild(lineGenre);
        item.appendChild(genre);
    }

    QString fichier = QFileDialog::getSaveFileName(0, "Exporter au format GCStar", QDir::homePath(), "Bases GCStar (*.gcstar)");
    if(fichier.isEmpty()){
        fichier = QDir::homePath()+"/save.gcstar";
    }
    if(!fichier.endsWith(".gcstar")){
        fichier.append(".gcstar");
    }
    QFile doc_xml(fichier);
    if (!doc_xml.open(QIODevice::WriteOnly)){
        return;
            }
    QTextStream sortie;
    sortie.setDevice(&doc_xml);
    doc.save(sortie, 4);
    doc_xml.close();

    QMessageBox::information(0, "Export effectué", "La base de données a été correctement exportée");
    return;
}

QList<QMultiMap<QString, QString> > XmlManager::getElementsByLabel(QString label){
    //Le principe: on vire de la liste tout élément NE CONTENANT PAS le label
    QList<QMultiMap<QString, QString> > resultat = this->readBase();
    for(int i=0; i<resultat.count(); ++i){
        if(!resultat.at(i).value("etiquettes").contains(label)){
            resultat.removeAt(i);
            i--;
        }
    }
    return resultat;
}

QStringList XmlManager::getListEtiquettes(){
    QList<QMultiMap<QString, QString> > base = this->readBase();
    QStringList retour;
    for(int i=0; i<base.count(); ++i){
        QStringList temp;
        temp = base.at(i).value("etiquettes").split(",");
        for(int j=0; j<temp.count(); ++j){
            if(!retour.contains(temp.at(j).trimmed(), Qt::CaseInsensitive))
                if(!temp.at(j).trimmed().isEmpty())
                    retour.append(temp.at(j).trimmed());
        }
    }
    return retour;
}
