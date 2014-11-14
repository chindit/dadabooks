#include "dadabooks.h"
#include "ui_dadabooks.h"

//Constructeur
DadaBooks::DadaBooks(QWidget *parent) : QMainWindow(parent), ui(new Ui::DadaBooks){
    ui->setupUi(this);
    insPreviewBook = new PreviewBook;
    insSettingsManager = new SettingsManager;
    insAddBook = new AddBook;
    insSiteManager = new SiteManager;

    idOngletEdit = -1;

    //On vérifie si le programme est initialisé ou non
    //if(insSettingsManager->getSettings(Fichier).toString().isEmpty() && !insSettingsManager->getSettings(MariaDB).toBool()){
    if(!insSettingsManager->getSettings(Initialized).toBool()){
        FirstLaunch *insFirstLaunch = new FirstLaunch;
        insFirstLaunch->exec();
        delete insFirstLaunch;

        //On recharge les paramètres
        insSettingsManager->loadSettings();
        insSettingsManager->setSettings(Initialized, true);
        if(insSettingsManager->getSettings(Fichier).toString().isEmpty() && !insSettingsManager->getSettings(MariaDB).toBool()){
            this->openFile();
        }
    }

    if(insSettingsManager->getSettings(Xml).toBool())
        insXmlManager = new XmlManager();
    else
        insSqlManager = new SqlManager();

    insEditBook = new EditBook;
    insSettingsDialog = new SettingsDialog;

    //Connexion des signaux et des slots
    connect(ui->pushButton_add, SIGNAL(clicked()), insAddBook, SLOT(show()));
    connect(insAddBook, SIGNAL(searchInternet(QString,QString)), this, SLOT(rechercheInternet(QString,QString)));
    connect(ui->pushButton_random, SIGNAL(clicked()), this, SLOT(selectRandom()));
    connect(insPreviewBook, SIGNAL(bookSelected(QString)), this, SLOT(getBook(QString)));
    connect(ui->tabWidget, SIGNAL(tabCloseRequested(int)), this, SLOT(closeTab(int)));
    connect(insEditBook, SIGNAL(editDone(int)), this, SLOT(updateOnglet(int)));
    connect(insEditBook, SIGNAL(bookAdded()), this, SLOT(setListeLivres()));
    connect(ui->pushButton_options, SIGNAL(clicked()), insSettingsDialog, SLOT(show()));
    connect(ui->actionQuitter, SIGNAL(triggered()), this, SLOT(close()));
    connect(ui->action_propos_de_DadaBooks, SIGNAL(triggered()), this, SLOT(about()));
    connect(ui->action_propos_de_Qt, SIGNAL(triggered()), qApp, SLOT(aboutQt()));
    connect(ui->actionNouveau, SIGNAL(triggered()), this, SLOT(openNewColl()));
    connect(ui->actionHTML, SIGNAL(triggered()), this, SLOT(exportAsHTML()));
    connect(ui->actionPDF, SIGNAL(triggered()), this, SLOT(exportAsPDF()));

    //Et on liste les livres
    this->setListeLivres();
}

//Destructeur
DadaBooks::~DadaBooks(){
    delete ui;
    delete insAddBook;
    delete insSiteManager;
    delete insPreviewBook;
    delete insEditBook;
    delete insSettingsDialog;
    if(insSettingsManager->getSettings(Xml).toBool())
           delete insXmlManager;
       else
           delete insSqlManager;
    delete insSettingsManager;
}

//Transfère les données de AddBook à PreviewBook
void DadaBooks::rechercheInternet(QString requete, QString site){
    insAddBook->close();
    QList< QMultiMap<QString,QString> > resultats;
    resultats = insSiteManager->makeSearch(requete, site);
    nomSiteRecherche = site;
    //On envoie à la fenêtre d'aperçu
    insPreviewBook->setTable(resultats);
    insPreviewBook->show();
    return;
}

//Sélectionne le livre fourni en paramètre
void DadaBooks::getBook(QString id){
    insPreviewBook->close();
    QMultiMap<QString,QString> livre;
    livre = insSiteManager->getBook(id, nomSiteRecherche);
    insEditBook->updateUi(livre);
    insEditBook->show();
}

//Actualise l'onglet courant
void DadaBooks::updateOnglet(int id){
    //On ne fait rien s'il s'agit de l'onglet principal
    if(insSettingsManager->getSettings(OpenInTab).toBool()){
        return;
    }
    if(idOngletEdit >= 0){
        ui->tabWidget->removeTab(idOngletEdit);
        this->activatePreview(id, false, true);
        ui->tabWidget->setCurrentIndex(idOngletEdit);
        idOngletEdit = -1;
    }
    return;
}

//Génère la liste livres totale et la liste des récents
void DadaBooks::setListeLivres(){
    QList<QMultiMap<QString, QString> > resultat;
    QSqlQuery res1;
    bool films = false;
    //XML
    if(insSettingsManager->getSettings(Xml).toBool()){
        insXmlManager = new XmlManager;
        resultat = insXmlManager->readBase();
    }
    else{
        //SQL
        QString req1;
        if(insSettingsManager->getSettings(Type).toString() == "livres")
            req1 = "SELECT livres.id, livres.titre, livres.annee, auteurs.nom, editeurs.nom AS nom_editeur FROM livres LEFT JOIN auteurs ON livres.auteur = auteurs.id LEFT JOIN editeurs ON livres.editeur = editeurs.id ORDER BY livres.id DESC LIMIT 0, 25";
        else{
            req1 = "SELECT * FROM films ORDER BY id DESC LIMIT 0, 25";
            films = true;
        }
        res1 = insSqlManager->query(req1);
    }

    //Nettoyage & préparation du widget
    while(ui->tableWidget->rowCount() > 0){
        ui->tableWidget->removeRow(0);
    }
    ui->tableWidget->setColumnCount(5);
    QStringList liste_headers;
    liste_headers << "ID" << "Titre" << ((QString::compare(insSettingsManager->getSettings(Type).toString(), "films", Qt::CaseInsensitive) != 0) ? "Auteur" : "Directeur") << ((QString::compare(insSettingsManager->getSettings(Type).toString(), "films", Qt::CaseInsensitive) != 0) ? "Éditeur" : "Acteurs") << "Année";
    ui->tableWidget->setHorizontalHeaderLabels(liste_headers);

    if(insSettingsManager->getSettings(Xml).toBool()){  //Dans le cas où la BDD est en XML
        int boucle = 25;
        if(resultat.size() < 25)
            boucle = resultat.size();
        for(int i=0; i<boucle; i++){
            ui->tableWidget->insertRow(0);
            QTableWidgetItem *item0 = new QTableWidgetItem();
            QTableWidgetItem *item1 = new QTableWidgetItem();
            QTableWidgetItem *item2 = new QTableWidgetItem();
            QTableWidgetItem *item3 = new QTableWidgetItem();
            QTableWidgetItem *item4 = new QTableWidgetItem();
            item0->setText(resultat.at(resultat.size()-1-i).value("id"));
            item1->setText(resultat.at(resultat.size()-1-i).value("titre"));
            if(QString::compare(insSettingsManager->getSettings(Type).toString(), "films", Qt::CaseInsensitive) != 0){
                item2->setText(resultat.at(resultat.size()-1-i).value("auteur"));
                item3->setText(resultat.at(resultat.size()-1-i).value("editeur"));
            }
            else{
                item2->setText(resultat.at(resultat.size()-1-i).value("directeur"));
                item3->setText(resultat.at(resultat.size()-1-i).value("acteurs"));
            }
            item4->setText(resultat.at(resultat.size()-1-i).value("annee"));
            ui->tableWidget->setItem(0, 0, item0);
            ui->tableWidget->setItem(0, 1, item1);
            ui->tableWidget->setItem(0, 2, item2);
            ui->tableWidget->setItem(0, 3, item3);
            ui->tableWidget->setItem(0, 4, item4);
        }
        //On remplit l'accueil
        ui->listWidget_accueil->clear();
        for(int i=0; i<resultat.size(); i++){
            ui->listWidget_accueil->addItem(resultat.at(i).value("titre"));
        }
        ui->listWidget_accueil->sortItems();
    }
    else{
        while(res1.next()){  //Dans le cas où la BDD est en SQL (lite)
            ui->tableWidget->insertRow(0);
            QTableWidgetItem *item0 = new QTableWidgetItem();
            QTableWidgetItem *item1 = new QTableWidgetItem();
            QTableWidgetItem *item2 = new QTableWidgetItem();
            QTableWidgetItem *item3 = new QTableWidgetItem();
            QTableWidgetItem *item4 = new QTableWidgetItem();
            item0->setText(res1.record().value("id").toString());
            item1->setText(res1.record().value("titre").toString());
            item2->setText(res1.record().value((films) ? "directeur" : "nom").toString());
            item3->setText(res1.record().value((films) ? "acteurs" : "nom_editeur").toString());
            item4->setText(res1.record().value("annee").toString());
            ui->tableWidget->setItem(0, 0, item0);
            ui->tableWidget->setItem(0, 1, item1);
            ui->tableWidget->setItem(0, 2, item2);
            ui->tableWidget->setItem(0, 3, item3);
            ui->tableWidget->setItem(0, 4, item4);
        }
        //Et l'accueil
        QString req2 = (films) ? "SELECT films.titre FROM films ORDER BY films.titre" : "SELECT livres.titre FROM livres ORDER BY livres.titre";
        QSqlQuery res2 = insSqlManager->query(req2);
        ui->listWidget_accueil->clear();
        while(res2.next()){
            ui->listWidget_accueil->addItem(res2.record().value("titre").toString());
        }
    }

    ui->tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);


    //Connexion des signaux/slots
    connect(ui->tableWidget, SIGNAL(cellDoubleClicked(int,int)), this, SLOT(activatePreview()));
    connect(ui->listWidget_accueil, SIGNAL(itemSelectionChanged()), this, SLOT(preparePreview()));
    ui->tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    return;
}

//Transfère le livre de la liste des récents à la prévisualisation en onglet
void DadaBooks::preparePreview(){
    //Le but de cette fonction est simplement de convertir l'ID d'index du QTableView en un ID de livre pour appeler ensuite activatePreview()
    int id = 0;
    if(insSettingsManager->getSettings(Xml).toBool()){
        if(!ui->listWidget_accueil->selectedItems().isEmpty())
            id = insXmlManager->getIdByTitle(ui->listWidget_accueil->selectedItems().first()->text());
    }
    else{
        QSqlQuery resultat;
        if(insSettingsManager->getSettings(Type).toString() == "livres")
            resultat = insSqlManager->query("SELECT id FROM livres WHERE titre=\""+ui->listWidget_accueil->currentItem()->text().replace("\"", "\\\"")+"\"");
        else
            resultat = insSqlManager->query("SELECT id FROM films WHERE titre=\""+ui->listWidget_accueil->currentItem()->text().replace("\"", "\\\"")+"\"");
        resultat.first();
        id = resultat.record().value("id").toInt();
    }
    if(id > 0){
        this->activatePreview(id, false, true);
    }
}

//Affiche le livre sélectionné dans un nouvel onglet
void DadaBooks::activatePreview(int id, bool search, bool idOk){
    if(idOk && insSettingsManager->getSettings(OpenInTab).toBool()){
        this->intabPreview(id);
        return;
    }
    QString titre;
    QMultiMap<QString, QString> xmlLivre;
    QSqlQuery res1;

    if(id == 0){
        if(!search){
        titre = ui->tableWidget->item(ui->tableWidget->currentRow(), 1)->text();
        }

        if(titre.size() > 25){
            titre.resize(25);
            titre.append("…");
        }
        for(int i=0; i<ui->tabWidget->count(); i++){
            if(ui->tabWidget->tabText(i) == titre){
                return; //Interdiction d'ouvrir 2x le même onglet
            }
        }
    }
    QWidget *nv_onglet = new QWidget(ui->tabWidget);

    //On regarde s'il s'agit d'un film ou d'un livre
    bool films = ((QString::compare(insSettingsManager->getSettings(Type).toString(), "films", Qt::CaseInsensitive) != 0) ? false : true);
    QGridLayout *layout_onglet = new QGridLayout(nv_onglet);
    nv_onglet->setLayout(layout_onglet);

    QString req1;
    if(!films)
        req1 = "SELECT livres.id, livres.titre, livres.ISBN, livres.coauteurs, livres.synopsis, livres.couverture, livres.pages, livres.edition, livres.langue, livres.classement, livres.exemplaires, livres.commentaire, livres.lu, livres.note, livres.empruntable, livres.pret, livres.annee, auteurs.nom, editeurs.nom AS nom_editeur FROM livres LEFT JOIN auteurs ON livres.auteur = auteurs.id LEFT JOIN editeurs ON livres.editeur = editeurs.id WHERE livres.id = ";
    else
        req1 = "SELECT * FROM films WHERE films.id=";
    QString real_id;
    if(search || idOk){
        real_id = QString::number(id)+";";
    }
    //Si ce n'est pas de la recherche, on suppose que l'utilisateur a voulu ouvrir un livre récent (ui->tableWidget).
    //Seulement, s'il a cliqué depuis ui->listWidget_accueil, on se retrouve avec une SIGSEGV vu qu'aucune ligne n'est sélectionnée
    //dans ui->tableWidget.  On vérifie donc si la propriété idOk est à TRUE.  Si c'est le cas, on a pas besoin de chercher le real_id,
    //il est déjà bon.
    else{
        real_id = ui->tableWidget->item(ui->tableWidget->currentRow(), 0)->text()+";";
    }

    if(!insSettingsManager->getSettings(Xml).toBool()){
        req1 += real_id;
        res1 = insSqlManager->query(req1);
        res1.next();
    }
    else{
        real_id.resize(real_id.size()-1);
        xmlLivre = insXmlManager->getBook(real_id.toInt());
    }

    //Recherche, on récupère le titre
    if(search){
        if(!insSettingsManager->getSettings(Xml).toBool()){
            titre = res1.record().value("titre").toString();
        }
        else{
            titre = xmlLivre.value("titre");
        }
        if(titre.size() > 25){
            titre.resize(25);
            titre.append("…");
        }
        for(int i=0; i<ui->tabWidget->count(); i++){
            if(ui->tabWidget->tabText(i) == titre){
                return; //Interdiction d'ouvrir 2x le même onglet
            }
        }
    }

    //Titre au cas où c'est une actualisation
    if(id > 0){
        if(!insSettingsManager->getSettings(Xml).toBool())
            titre = res1.record().value("titre").toString();
        else
            titre = xmlLivre.value("titre");
        if(titre.size() > 25){
            titre.resize(25);
            titre.append("…");
        }
    }
    ui->tabWidget->addTab(nv_onglet, titre);

    //-----------------------------------------------
    //Création des éléments
    //-----------------------------------------------
    QLabel *titre_fen, *id1, *id2, *titre_livre, *titre2, *isbn, *isbn2, *coauteurs, *coauteurs2, *synopsis, *couverture, *couverture2, *pages, *pages2, *edition, *edition2, *langue, *langue2, *classement, *classement2, *exemplaires, *exemplaires2, *commentaire, *annee, *annee2, *auteur, *auteur2, *editeur, *editeur2, *note1, *note2, *ebook2, *ebook3;
    id1 = new QLabel("ID");
    titre_livre = new QLabel("Titre");
    isbn = new QLabel((films) ? "Genre" : "ISBN");
    coauteurs = new QLabel((films) ? "Titre original" : "Co-auteur(s)");
    synopsis = new QLabel("Résumé");
    couverture = new QLabel("Couverture");
    couverture2 = new QLabel();
    QPixmap loader;
    pages = new QLabel((films) ? "Durée" : "Nombre de pages");
    edition = new QLabel((films) ? "Nationalité" : "Édition");
    langue = new QLabel((films) ? "Langue" : "Langue de publication");
    exemplaires = new QLabel((films) ? "Sous-titres" : "Nombre d'exemplaires");
    commentaire = new QLabel("Commentaire");
    classement = new QLabel("Classement");
    annee = new QLabel((films) ? "Année de production" : "Année de publication");
    auteur = new QLabel((films) ? "Directeur" : "Auteur");
    editeur = new QLabel((films) ? "Acteurs" : "Éditeur");
    note1 = new QLabel("Note");
    ebook2 = new QLabel("Emplacement");
    QCheckBox *empruntable, *prete, *lu, *ebook1;
    empruntable = new QCheckBox("Empruntable");
    empruntable->setEnabled(false);
    prete = new QCheckBox("En prêt");
    prete->setEnabled(false);
    lu = new QCheckBox((films) ? "Vu" : "Lu");
    lu->setEnabled(false);
    ebook1 = new QCheckBox((films) ? "Fichier" : "Ebook");
    ebook1->setEnabled(false);
    QSpacerItem *spacer = new QSpacerItem(10, 100, QSizePolicy::Minimum, QSizePolicy::Minimum);
    QPushButton *button_edit = new QPushButton;
    button_edit->setFlat(true);
    button_edit->setIcon(QIcon(":/boutons/images/edit.png"));
    QPushButton *button_delete = new QPushButton;
    button_delete->setFlat(true);
    button_delete->setIcon(QIcon(":/boutons/images/delete.png"));
    QTextEdit *synopsis2 = new QTextEdit;
    QTextEdit *commentaire2 = new QTextEdit;
    QHBoxLayout *layoutEtiquettes = new QHBoxLayout;

    QSignalMapper *mapper_delete = new QSignalMapper;
    connect(button_delete, SIGNAL(clicked()), mapper_delete, SLOT(map()));
    QSignalMapper *mapper_edit = new QSignalMapper;
    connect(button_edit, SIGNAL(clicked()), mapper_edit, SLOT(map()));

    if(insSettingsManager->getSettings(Xml).toBool()){
        //XML
        titre_fen = new QLabel(xmlLivre.value("titre"));
        id2 = new QLabel(xmlLivre.value("id"));
        titre2 = new QLabel(xmlLivre.value("titre"));
        isbn2 = new QLabel(xmlLivre.value((films) ? "genre" : "isbn"));
        coauteurs2 = new QLabel(xmlLivre.value((films) ? "titreOriginal" : "coauteurs"));
        if(!xmlLivre.value("couverture").startsWith("http") && !xmlLivre.value("couverture").isEmpty()){
            QFile fichierImage(xmlLivre.value("couverture"));
            if(!fichierImage.exists()){
                QMessageBox::information(this, "Image introuvable", "Une erreur est survenue, la jaquette de ce livre ne peut être trouvée");
            }
            else{
                loader.load(xmlLivre.value("couverture"));
            }
        }
        else{
            QNetworkAccessManager nw_manager;
            QNetworkRequest request(xmlLivre.value("couverture"));
            QNetworkReply *reponse = nw_manager.get(request);
            QEventLoop eventLoop;
            connect(reponse, SIGNAL(finished()), &eventLoop, SLOT(quit()));
            eventLoop.exec();
            QByteArray data = reponse->readAll();
            loader.loadFromData(data);
        }
        int width = loader.width();
        if(width > 150){
            float coef = (float)width / 150;
            int result = loader.width()/coef;
            loader = loader.scaledToWidth(result);
        }
        couverture2->setPixmap(loader);
        pages2 = new QLabel(xmlLivre.value((films) ? "duree" : "pages"));
        edition2 = new QLabel(xmlLivre.value((films) ? "pays" : "edition"));
        langue2 = new QLabel(xmlLivre.value("langue"));
        exemplaires2 = new QLabel(xmlLivre.value((films) ? "sousTitres" : "exemplaires"));
        classement2 = new QLabel(xmlLivre.value("classement"));
        annee2 = new QLabel(xmlLivre.value("annee"));
        auteur2 = new QLabel(xmlLivre.value((films) ? "directeur" : "auteur"));
        editeur2 = new  QLabel(xmlLivre.value((films) ? "acteurs" : "editeur"));
        if(editeur2->text().size() > 50)
            editeur2->setText(editeur2->text().left(editeur2->text().left(50).lastIndexOf(","))); //On coupe après le dernier nom qui fait moins de 50 caractères
        note2 = new QLabel(xmlLivre.value("note")+"/20");
        ebook3 = new QLabel(xmlLivre.value("emplacement", ""));
        synopsis2->insertPlainText(xmlLivre.value("synopsis"));
        commentaire2->insertPlainText(xmlLivre.value("commentaire"));
        prete->setChecked(((xmlLivre.value("prete") == "True") ? true : false));
        empruntable->setChecked(((xmlLivre.value("empruntable") == "True") ? true : false));
        ebook1->setChecked(((xmlLivre.value((films) ? "fichier" : "ebook") == "True") ? true : false));
        mapper_delete->setMapping(button_delete, xmlLivre.value("id").toInt());
        mapper_edit->setMapping(button_edit, xmlLivre.value("id").toInt());
        lu->setChecked(((xmlLivre.value((films) ? "vu" : "lu") == "True") ? true : false));
    }
    else{
        //SQL
        titre_fen = new QLabel(res1.record().value("titre").toString());
        id2 = new QLabel(res1.record().value("id").toString());
        titre2 = new QLabel(res1.record().value("titre").toString());
        isbn2 = new QLabel(res1.record().value(((films) ? "genre" : "isbn")).toString());
        coauteurs2 = new QLabel(res1.record().value(((films) ? "titre_original" : "coauteurs")).toString());
        if(!res1.record().value((films) ? "jaquette" : "couverture").toString().startsWith("http")){
            QFile fichierImage(res1.record().value((films) ? "jaquette" : "couverture").toString());
            if(!fichierImage.exists() && !res1.record().value((films) ? "jaquette" : "couverture").toString().isEmpty()){
                QMessageBox::information(this, "Image introuvable", "Une erreur est survenue, la jaquette de ce livre ne peut être trouvée");
            }
            else{
                loader.load(res1.record().value((films) ? "jaquette" : "couverture").toString());
            }
        }
        else{
            QNetworkAccessManager nw_manager;
            QNetworkRequest request(res1.record().value((films) ? "jaquette" : "couverture").toString());
            QNetworkReply *reponse = nw_manager.get(request);
            QEventLoop eventLoop;
            connect(reponse, SIGNAL(finished()), &eventLoop, SLOT(quit()));
            eventLoop.exec();
            QByteArray data = reponse->readAll();
            loader.loadFromData(data);
        }
        int width = loader.width();
        if(width > 150){
            float coef = (float)width / 150;
            int result = loader.width()/coef;
            loader = loader.scaledToWidth(result);
        }
        couverture2->setPixmap(loader);
        pages2 = new QLabel(res1.record().value((films) ? "duree" : "pages").toString());
        edition2 = new QLabel(res1.record().value((films) ? "pays" : "edition").toString());
        langue2 = new QLabel(res1.record().value("langue").toString());
        exemplaires2 = new QLabel(res1.record().value((films) ? "sous_titres" : "exemplaires").toString());
        classement2 = new QLabel(res1.record().value("classement").toString());
        annee2 = new QLabel(res1.record().value("annee").toString());
        auteur2 = new QLabel(res1.record().value((films) ? "directeur" : "nom").toString());
        editeur2 = new QLabel(res1.record().value((films) ? "acteurs" : "nom_editeur").toString());
        if(editeur2->text().size() > 50)
            editeur2->setText(editeur2->text().left(editeur2->text().left(50).lastIndexOf(","))); //On coupe après le dernier nom qui fait moins de 50 caractères
        note2 = new QLabel(res1.record().value("note").toString()+"/20");
        synopsis2->insertPlainText(res1.record().value("synopsis").toString());
        commentaire2->insertPlainText(res1.record().value("commentaire").toString());
        ebook3 = new QLabel(res1.record().value("emplacement").toString());
        empruntable->setChecked(res1.record().value("empruntable").toBool());
        prete->setChecked(res1.record().value("pret").toBool());
        ebook1->setChecked(res1.record().value((films) ? "fichier" : "ebook").toBool());
        mapper_delete->setMapping(button_delete, res1.record().value("id").toInt());
        mapper_edit->setMapping(button_edit, res1.record().value("id").toInt());

        //Stockage des étiquettes dans un layout propre
        QSqlQuery liste = insSqlManager->query("SELECT etiquettes.id,etiquettes.nom FROM etiquettes LEFT JOIN liste_etiquettes ON etiquettes.id=liste_etiquettes.id_etiquette WHERE liste_etiquettes.id_livre="+real_id);
        while(liste.next()){
            QPushButton *etiquette = new QPushButton(liste.record().value("nom").toString());
            etiquette->setFlat(true);
            etiquette->setIcon(QIcon(":/boutons/images/etiquette.png"));
            etiquette->setStyleSheet("text-decoration: underline;");
            QSignalMapper *mappeurEtiquette = new QSignalMapper;
            connect(etiquette, SIGNAL(clicked()), mappeurEtiquette, SLOT(map()));
            mappeurEtiquette->setMapping(etiquette, liste.record().value("id").toInt());
            connect(mappeurEtiquette, SIGNAL(mapped(const int &)), this, SLOT(showEtiquette(const int &)));
            layoutEtiquettes->addWidget(etiquette);
        }
    }

    synopsis2->setReadOnly(true);
    commentaire2->setReadOnly(true);
    titre_fen->setFont(QFont("Ubuntu", 20, 0, false));

    connect(mapper_delete, SIGNAL(mapped(int)), this, SLOT(deleteLivre(int)));
    connect(mapper_edit, SIGNAL(mapped(int)), this, SLOT(editLivre(int)));

    //-----------------------------------------------
    //Mise en place dans le layout
    //-----------------------------------------------
    layout_onglet->addWidget(titre_fen, 0, 0, 1, 4, Qt::AlignHCenter);
    QHBoxLayout *layout_buttons = new QHBoxLayout;
    layout_buttons->addWidget(button_edit, 0, Qt::AlignRight | Qt::AlignTop);
    layout_buttons->addWidget(button_delete, 0, Qt::AlignRight | Qt::AlignTop);
    layout_onglet->addLayout(layout_buttons, 0, 4, 1, 1, Qt::AlignRight | Qt::AlignTop);
    QHBoxLayout *layout_id = new QHBoxLayout;
    layout_id->addWidget(id1);
    layout_id->addWidget(id2);
    layout_onglet->addLayout(layout_id, 2, 1, 1, 2, Qt::AlignLeft);
    layout_onglet->addWidget(titre_livre, 2, 3);
    layout_onglet->addWidget(titre2, 2, 4);
    layout_onglet->addWidget(isbn, 7, 3);
    layout_onglet->addWidget(isbn2, 7, 4);
    layout_onglet->addWidget(coauteurs, 3, 3);
    layout_onglet->addWidget(coauteurs2, 3, 4);
    layout_onglet->addWidget(synopsis, 12, 0);
    layout_onglet->addWidget(synopsis2, 13, 0, 1, 3);
    layout_onglet->addWidget(pages, 5, 1);
    layout_onglet->addWidget(pages2, 5, 2);
    layout_onglet->addWidget(edition, 5, 3);
    layout_onglet->addWidget(edition2, 5, 4);
    layout_onglet->addWidget(langue, 6, 1);
    layout_onglet->addWidget(langue2, 6, 2);
    layout_onglet->addWidget(exemplaires, 6, 3);
    layout_onglet->addWidget(exemplaires2, 6, 4);
    layout_onglet->addWidget(commentaire, 12, 3, 1, 1, Qt::AlignLeft);
    layout_onglet->addWidget(commentaire2, 13, 3, 1, 2);
    layout_onglet->addWidget(classement, 7, 1);
    layout_onglet->addWidget(classement2, 7, 2);
    layout_onglet->addWidget(annee, 4, 3);
    layout_onglet->addWidget(annee2, 4, 4);
    layout_onglet->addWidget(auteur, 3, 1);
    layout_onglet->addWidget(auteur2, 3, 2);
    layout_onglet->addWidget(editeur, 4, 1);
    layout_onglet->addWidget(editeur2, 4, 2);
    layout_onglet->addWidget(empruntable, 8, 1, 1, 2, Qt::AlignLeft);
    layout_onglet->addWidget(prete, 8, 2, 1, 1, Qt::AlignLeft);
    layout_onglet->addWidget(lu, 8, 3, 1, 2, Qt::AlignLeft);
    layout_onglet->addWidget(ebook1, 9, 1, 1, 2, Qt::AlignLeft);
    layout_onglet->addWidget(ebook2, 9, 2, 1, 1, Qt::AlignLeft);
    layout_onglet->addWidget(ebook3, 9, 3, 1, 2, Qt::AlignLeft);
    layout_onglet->addWidget(couverture, 1, 0, 1, 1, Qt::AlignHCenter);
    layout_onglet->addWidget(couverture2, 2, 0, 10, 1, Qt::AlignVCenter);
    layout_onglet->addWidget(note1, 10, 1);
    layout_onglet->addWidget(note2, 10, 2);
    layout_onglet->addLayout(layoutEtiquettes, 10, 3, 1, 2, Qt::AlignLeft);

    layout_onglet->addItem(spacer, 11, 1, 1, 3, Qt::AlignHCenter);
    ui->tabWidget->setCurrentWidget(nv_onglet);
    return;
}

//Ferme l'onglet sélectionné
void DadaBooks::closeTab(int tab){
    if(ui->tabWidget->tabText(tab) == "Accueil" || ui->tabWidget->tabText(tab) == "Récents"){
        return;
    }
    else{
        ui->tabWidget->removeTab(tab);
    }
}

//Appelle EditBook avec le contenu du livre fourni en paramètre
void DadaBooks::editLivre(int id){
    idOngletEdit = ui->tabWidget->currentIndex();
    QMultiMap<QString, QString> livre;
    QString req1;
    bool films = (insSettingsManager->getSettings(Type).toString() == "films") ? true : false;
    if(!films)
        req1 = "SELECT livres.id, livres.titre, livres.ISBN, livres.coauteurs, livres.synopsis, livres.couverture, livres.pages, livres.edition, livres.langue, livres.classement, livres.exemplaires, livres.commentaire, livres.note, livres.lu, livres.empruntable, livres.pret, livres.ebook, livres.emplacement, livres.annee, auteurs.nom, editeurs.nom AS nom_editeur FROM livres LEFT JOIN auteurs ON livres.auteur = auteurs.id LEFT JOIN editeurs ON livres.editeur = editeurs.id WHERE livres.id = "+QString::number(id)+";";
    else
        req1 = "SELECT * FROM films WHERE films.id="+QString::number(id)+";";

    if(insSettingsManager->getSettings(Xml).toBool()){
        //XML
        livre = insXmlManager->getBook(id);
        livre.insert("xml", "1");
    }
    else{
        QSqlQuery res1 = insSqlManager->query(req1);
        res1.next();
        livre.insert("couverture", res1.record().value((films) ? "jaquette" : "couverture").toString());
        livre.insert("titre", res1.record().value("titre").toString());
        livre.insert((films) ? "directeur" : "auteur", res1.record().value((films) ? "directeur" : "nom").toString());
        livre.insert((films) ? "acteurs" : "editeur", res1.record().value((films) ? "acteurs" : "nom_editeur").toString());
        livre.insert("annee", res1.record().value("annee").toString());
        livre.insert((films) ? "genre": "isbn", res1.record().value((films) ? "genre" : "isbn").toString());
        livre.insert("langue", res1.record().value("langue").toString());
        livre.insert((films) ? "duree" : "pages", res1.record().value((films) ? "duree" : "pages").toString());
        livre.insert("synopsis", res1.record().value("synopsis").toString());
        livre.insert((films) ? "titreOriginal" : "coauteurs", res1.record().value((films) ? "titre_original" : "coauteurs").toString());
        livre.insert((films) ? "pays" : "edition", res1.record().value((films) ? "pays" : "edition").toString());
        livre.insert((films) ? "sousTitres" : "exemplaires", res1.record().value((films) ? "sous_titres" : "exemplaires").toString());
        livre.insert("commentaire", res1.record().value("commentaire").toString());
        livre.insert("classement", res1.record().value("classement").toString());
        livre.insert("empruntable", res1.record().value("empruntable").toString());
        livre.insert("prete", res1.record().value("pret").toString());
        livre.insert("note", res1.record().value("note").toString());
        livre.insert("lu", res1.record().value("lu").toString());
        livre.insert("ebook", res1.record().value((films) ? "fichier" : "ebook").toString());
        livre.insert("emplacement", res1.record().value("emplacement").toString());
        livre.insert("xml", "0");
    }
    insEditBook->setId(id);
    insEditBook->updateUi(livre);
    insEditBook->show();
    return;
}

//Supprime le livre fourni en paramètre
void DadaBooks::deleteLivre(int id){
    QString titre;
    if(insSettingsManager->getSettings(Xml).toBool()){
        titre = insXmlManager->getBook(id).value("titre");
    }
    else{
        QString req1;
        if(insSettingsManager->getSettings(Type).toString() == "livres")
            req1 = "SELECT titre FROM livres WHERE id = "+QString::number(id)+";";
        else
            req1 = "SELECT titre FROM films WHERE id = "+QString::number(id)+";";
        QSqlQuery res1 = insSqlManager->query(req1);
        res1.next();
        titre = res1.record().value("titre").toString();
    }
    QString titreBoite = QString("Supprimer un ")+((insSettingsManager->getSettings(Type).toString() == "livres") ? "livre" : "film");
    int reponse = QMessageBox::question(this, titreBoite, QString("Êtes-vous sûr de vouloir supprimer <i>%1</i>?").arg(titre), QMessageBox::Yes | QMessageBox::No);
    if(reponse == QMessageBox::Yes){
        if(!insSettingsManager->getSettings(Xml).toBool()){
            QString req1;
            if(insSettingsManager->getSettings(Type).toString() == "livres")
                req1 = QString("DELETE FROM livres WHERE id = %1").arg(id);
            else
                req1 = QString("DELETE FROM films WHERE id = %1").arg(id);
            QSqlQuery res1 = insSqlManager->query(req1);
            res1.exec(req1);
        }
        else{
            insXmlManager->deleteBook(id);
        }

        //On met à jour le tableau
        this->setListeLivres();
        //On vire l'onglet
        this->closeTab(ui->tabWidget->currentIndex());
    }

    //Si on est à l'accueil, on change le QStackedWidget sinon, on ferme l'onglet
    if(ui->tabWidget->tabText(ui->tabWidget->currentIndex()) == "Accueil")
        ui->stackedWidget->setCurrentIndex(0);
    else
        this->closeTab(ui->tabWidget->currentIndex());
    return;
}

//Recherche toutes les occurences possibles à partir du champ de recherche
void DadaBooks::makeSearch(){
    QList<QMultiMap<QString, QString> > resultats;
    QSqlQuery res1;

    QString recherche = ui->lineEdit_search->text();
    if(recherche.isEmpty() || recherche.isNull()){
        QMessageBox::warning(this, "Chaîne vide", "Aucun terme recherché");
        return;
    }
    QStringList termes = recherche.split(" ");
    if(!insSettingsManager->getSettings(Xml).toBool()){
        QString requete, temp;
        if(insSettingsManager->getSettings(Type).toString() == "livres"){
            requete = "SELECT livres.id, livres.titre, livres.annee, auteurs.nom, editeurs.nom AS nom_editeur FROM livres LEFT JOIN auteurs ON livres.auteur = auteurs.id LEFT JOIN editeurs ON livres.editeur = editeurs.id WHERE";
            for(int i=0; i<termes.size(); i++){
                if(termes.at(i).size() > 3){
                    temp.append(" OR (livres.titre LIKE \"%");
                    temp.append(termes.at(i));
                    temp.append("%\") OR (livres.annee LIKE \"%");
                    temp.append(termes.at(i));
                    temp.append("%\") OR (auteurs.nom LIKE \"%");
                    temp.append(termes.at(i));
                    temp.append("%\") OR (editeurs.nom LIKE \"%");
                    temp.append(termes.at(i));
                    temp.append("%\") OR (livres.synopsis LIKE \"%");
                    temp.append(termes.at(i));
                    temp.append("%\")");
                }
            }
        }
        else{
            requete = "SELECT films.id, films.titre, films.annee, films.directeur, films.acteurs FROM films WHERE";
            for(int i=0; i<termes.size(); ++i){
                if(termes.at(i).size() > 3){
                    temp.append(" OR (films.titre LIKE \"%");
                    temp.append(termes.at(i));
                    temp.append("%\") OR (films.annee LIKE \"%");
                    temp.append(termes.at(i));
                    temp.append("%\") OR (films.directeur LIKE \"%");
                    temp.append(termes.at(i));
                    temp.append("%\") OR (films.acteurs LIKE \"%");
                    temp.append(termes.at(i));
                    temp.append("%\") OR (films.synopsis LIKE \"%");
                    temp.append(termes.at(i));
                    temp.append("%\")");
                }
            }
        }
        temp = temp.right(temp.size()-3);
        requete.append(temp);
        requete.append(";");

        res1 = insSqlManager->query(requete);
    }
    else{
        resultats = insXmlManager->makeSearch(termes);
    }

    QWidget *nv_onglet = new QWidget(ui->tabWidget);
    ui->tabWidget->addTab(nv_onglet, "Recherche...");
    QTableWidget *table = new QTableWidget(nv_onglet);
    table->setColumnCount(5);
    QStringList liste_headers;
    bool films = ((QString::compare(insSettingsManager->getSettings(Type).toString(), "films", Qt::CaseInsensitive) != 0) ? false : true);
    liste_headers << "ID" << "Titre" << ((films) ? "Directeur" : "Auteur") << ((films) ? "Acteurs" : "Éditeur") << "Année";
    table->setHorizontalHeaderLabels(liste_headers);
    QGridLayout *layout = new QGridLayout;
    layout->addWidget(table, 0, 0);
    nv_onglet->setLayout(layout);

    if(!insSettingsManager->getSettings(Xml).toBool()){
        while(res1.next()){
            table->insertRow(0);
            QTableWidgetItem *item0 = new QTableWidgetItem();
            QTableWidgetItem *item1 = new QTableWidgetItem();
            QTableWidgetItem *item2 = new QTableWidgetItem();
            QTableWidgetItem *item3 = new QTableWidgetItem();
            QTableWidgetItem *item4 = new QTableWidgetItem();
            item0->setText(res1.record().value("id").toString());
            item1->setText(res1.record().value("titre").toString());
            item2->setText(res1.record().value((films) ? "directeur" : "nom").toString());
            item3->setText(res1.record().value((films) ? "acteurs" : "nom_editeur").toString());
            item4->setText(res1.record().value("annee").toString());
            table->setItem(0, 0, item0);
            table->setItem(0, 1, item1);
            table->setItem(0, 2, item2);
            table->setItem(0, 3, item3);
            table->setItem(0, 4, item4);
        }
    }
    else{
        for(int i=0; i<resultats.size(); i++){
            table->insertRow(0);
            QTableWidgetItem *item0 = new QTableWidgetItem();
            QTableWidgetItem *item1 = new QTableWidgetItem();
            QTableWidgetItem *item2 = new QTableWidgetItem();
            QTableWidgetItem *item3 = new QTableWidgetItem();
            QTableWidgetItem *item4 = new QTableWidgetItem();
            item0->setText(resultats.at(i).value("id"));
            item1->setText(resultats.at(i).value("titre"));
            item2->setText(resultats.at(i).value("auteur"));
            item3->setText(resultats.at(i).value("editeur"));
            item4->setText(resultats.at(i).value("annee"));
            table->setItem(0, 0, item0);
            table->setItem(0, 1, item1);
            table->setItem(0, 2, item2);
            table->setItem(0, 3, item3);
            table->setItem(0, 4, item4);
        }
    }
    table->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    connect(table, SIGNAL(cellDoubleClicked(int,int)), this, SLOT(prepareSearchView(int)));
    table->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tabWidget->setCurrentWidget(nv_onglet);
    return;
}

//Transfère le livre du résultat de recherche vers la prévisualisation
void DadaBooks::prepareSearchView(int row){
    QWidget *widget = ui->tabWidget->widget(ui->tabWidget->currentIndex());
    QList<QTableWidget *>liste = widget->findChildren<QTableWidget *>();
    if(liste.size() == 0){
        return;
    }
    QTableWidget *tWidget = liste.at(0);
    QTableWidgetItem *item = tWidget->item(row, 0);
    QString id = item->text();
    this->activatePreview(id.toInt(), true);
    return;
}

//Ouvre un fichier de base de données
void DadaBooks::openFile(){
    QString fileName = QFileDialog::getOpenFileName(this, "Ouvrir un fichier", QDir::homePath(), "Tous documents (*.sql, *.xml);;Documents XML (*.xml);;Bases de données (*.db)");
    if(fileName.endsWith(".xml")){
        insSettingsManager->setSettings(Fichier, fileName);
        insSettingsManager->setSettings(Sqlite, false);
        insSettingsManager->setSettings(MariaDB, false);
        insSettingsManager->setSettings(Xml, true);
        this->setListeLivres();
    }
    else if(fileName.endsWith(".db")){
        insSettingsManager->setSettings(Fichier, fileName);
        insSettingsManager->setSettings(Sqlite, true);
        insSettingsManager->setSettings(MariaDB, false);
        insSettingsManager->setSettings(Xml, false);
        this->setListeLivres();
    }
    else{
        //Do nothing
    }
    return;
}

//Boite de dialogue «À propos» du programme
void DadaBooks::about(){
    QString string_about = ("<h2>À propos de DadaBooks</h2><br><b>Dévoloppé par</b> : David Lumaye<br><b>Version</b> : ")+QString(VERSION)+tr("<br><b>Courriel</b>:<a href='mailto:d.lumaye.aro-base.mailtunnel.eu'>d.lumaye.aro-base.mailtunnel.eu</a><br><b>Distribué sous license</b> : <a href='http://www.gnu.org/licenses/gpl-3.0.fr.html'>GPL 3</a>");
    QMessageBox::about(this, tr("À propos de DadaBooks"), string_about);
    return;
}

//Ouvre ou crée une nouvelle collection
void DadaBooks::openNewColl(){
    //Pour créer une nouvelle collection, on vire la référence de l'ancienne
    insSettingsManager->setSettings(Fichier, "");
    insSettingsManager->setSettings(Xml, true);
    insSettingsManager->setSettings(MariaDB, false);
    insSettingsManager->setSettings(Sqlite, false);
    insSettingsManager->setSettings(Initialized, false);

    //Et on relance le programme
    QProcess::startDetached(QApplication::applicationFilePath());
    QCoreApplication::quit();
}

//Affiche les étiquettes du livre fourni en paramètre
void DadaBooks::showEtiquette(const int &id){
    QSqlQuery requete;
    bool films = insSettingsManager->getSettings(Type).toString() == "films";
    if(!films)
        requete = insSqlManager->query("SELECT livres.id, livres.titre, livres.annee, auteurs.nom, editeurs.nom AS nom_editeur FROM livres LEFT JOIN liste_etiquettes ON livres.id=liste_etiquettes.id_livre LEFT JOIN auteurs ON livres.auteur = auteurs.id LEFT JOIN editeurs ON livres.editeur = editeurs.id WHERE liste_etiquettes.id_etiquette="+QString::number(id)+" ORDER BY id DESC");
    else
        requete = insSqlManager->query("SELECT * FROM films LEFT JOIN liste_etiquettes ON films.id=liste_etiquettes.id_livre WHERE liste_etiquettes.id_etiquette="+QString::number(id)+" ORDER BY id DESC");

    QWidget *nv_onglet = new QWidget(ui->tabWidget);
    ui->tabWidget->addTab(nv_onglet, "Recherche...");
    QTableWidget *table = new QTableWidget(nv_onglet);
    table->setColumnCount(5);
    QStringList liste_headers;
    liste_headers << "ID" << "Titre" << ((films) ? "Directeur" : "Auteur") << ((films) ? "Acteurs" : "Éditeur") << "Année";
    table->setHorizontalHeaderLabels(liste_headers);
    QGridLayout *layout = new QGridLayout;
    layout->addWidget(table, 0, 0);
    nv_onglet->setLayout(layout);

    while(requete.next()){
        table->insertRow(0);
        QTableWidgetItem *item0 = new QTableWidgetItem();
        QTableWidgetItem *item1 = new QTableWidgetItem();
        QTableWidgetItem *item2 = new QTableWidgetItem();
        QTableWidgetItem *item3 = new QTableWidgetItem();
        QTableWidgetItem *item4 = new QTableWidgetItem();
        item0->setText(requete.record().value("id").toString());
        item1->setText(requete.record().value("titre").toString());
        item2->setText(requete.record().value((films) ? "directeur" : "nom").toString());
        item3->setText(requete.record().value((films) ? "acteurs" : "nom_editeur").toString());
        item4->setText(requete.record().value("annee").toString());
        table->setItem(0, 0, item0);
        table->setItem(0, 1, item1);
        table->setItem(0, 2, item2);
        table->setItem(0, 3, item3);
        table->setItem(0, 4, item4);
    }
    table->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    connect(table, SIGNAL(cellDoubleClicked(int,int)), this, SLOT(prepareSearchView(int)));
    table->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tabWidget->setCurrentWidget(nv_onglet);
    return;
}

//Affiche les éléments contenant l'étiquette fournie en paramètres
void DadaBooks::showEtiquette(const QString nom){
    if(insSettingsManager->getSettings(Xml).toBool()){ //XML
        QList< QMultiMap <QString, QString> > elements = insXmlManager->getElementsByLabel(nom);
        QWidget *nv_onglet = new QWidget(ui->tabWidget);
        ui->tabWidget->addTab(nv_onglet, "Recherche...");
        QTableWidget *table = new QTableWidget(nv_onglet);
        table->setColumnCount(5);
        QStringList liste_headers;
        liste_headers << "ID" << "Titre" << "Directeur" << "Acteurs" << "Année";
        table->setHorizontalHeaderLabels(liste_headers);
        QGridLayout *layout = new QGridLayout;
        layout->addWidget(table, 0, 0);
        nv_onglet->setLayout(layout);

        for(int i=0; i<elements.count(); ++i){
            table->insertRow(0);
            QTableWidgetItem *item0 = new QTableWidgetItem();
            QTableWidgetItem *item1 = new QTableWidgetItem();
            QTableWidgetItem *item2 = new QTableWidgetItem();
            QTableWidgetItem *item3 = new QTableWidgetItem();
            QTableWidgetItem *item4 = new QTableWidgetItem();
            item0->setText(elements.at(i).value("id"));
            item1->setText(elements.at(i).value("titre"));
            item2->setText(elements.at(i).value("directeur"));
            item3->setText(elements.at(i).value("acteurs"));
            item4->setText(elements.at(i).value("annee"));
            table->setItem(0, 0, item0);
            table->setItem(0, 1, item1);
            table->setItem(0, 2, item2);
            table->setItem(0, 3, item3);
            table->setItem(0, 4, item4);
        }
        table->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
        connect(table, SIGNAL(cellDoubleClicked(int,int)), this, SLOT(prepareSearchView(int)));
        table->setEditTriggers(QAbstractItemView::NoEditTriggers);
        ui->tabWidget->setCurrentWidget(nv_onglet);
    }
    else{ //SQL
        //TODO
    }
    return;
}

//Affiche le livre sélectionné dans l'onglet principal
void DadaBooks::intabPreview(int id){
    //Détection du type de collection
    bool films = ((QString::compare(insSettingsManager->getSettings(Type).toString(), "films", Qt::CaseInsensitive) != 0) ? false : true);
    bool sql = !insSettingsManager->getSettings(Xml).toBool();

    //Récupération des données
    QSqlQuery res1;
    QMultiMap<QString, QString> xmlLivre;
    if(sql){
        QString req1;
        if(!films)
            req1 = "SELECT livres.id, livres.titre, livres.ISBN, livres.coauteurs, livres.synopsis, livres.couverture, livres.pages, livres.edition, livres.langue, livres.classement, livres.exemplaires, livres.commentaire, livres.lu, livres.note, livres.empruntable, livres.pret, livres.annee, auteurs.nom, editeurs.nom AS nom_editeur FROM livres LEFT JOIN auteurs ON livres.auteur = auteurs.id LEFT JOIN editeurs ON livres.editeur = editeurs.id WHERE livres.id="+QString::number(id);
        else
            req1 = "SELECT * FROM films WHERE films.id="+QString::number(id);
        res1 = insSqlManager->query(req1);
        res1.first();
    }
    else{
        xmlLivre = insXmlManager->getBook(id);
    }

    if(films){
        ui->stackedWidget->setCurrentIndex(1);
    }
    else{
        ui->stackedWidget->setCurrentIndex(2);
    }

    //Connexion des signaux-slots disponibles
    mapperDelete = new QSignalMapper; mapperEdit = new QSignalMapper;
    connect(mapperDelete, SIGNAL(mapped(int)), this, SLOT(deleteLivre(int)));
    connect(mapperEdit, SIGNAL(mapped(int)), this, SLOT(editLivre(int)));

    if(films){
        //------------------------------
        //FILMS
        //------------------------------
        connect(ui->pushButtonListes, SIGNAL(clicked(bool)), this, SLOT(expandIntabContent(bool)));

        //Masquage des deux champs
        this->expandIntabContent(false); //L'appel au slot permet également de modifier le bouton en cas de besoin

        //Vidage des données anciennes.
        ui->listWidgetActeurs->clear();
        ui->listWidgetGenres->clear();
        ui->plainTextEditSynopsis->clear();
        ui->plainTextEditCommentaire->clear();

        //Remplissage des données
        ui->labelTitre->setText(((sql) ? res1.record().value("titre").toString() : xmlLivre.value("titre")));
        ui->labelID->setText(((sql) ? res1.record().value("id").toString() : xmlLivre.value("id")));
        ui->labelTitreOriginal->setText(((sql) ? res1.record().value("titre_original").toString() : xmlLivre.value("titreOriginal")));
        //Image
        QPixmap loader = ToolsManager::getPixmapFromString((sql) ? res1.record().value("jaquette").toString() : xmlLivre.value("couverture"));
        ui->labelImage->setPixmap(loader);
        ui->labelDirecteur->setText(((sql) ? res1.record().value("directeur").toString() : xmlLivre.value("directeur")));
        ui->labelAnnee->setText(((sql) ? res1.record().value("annee").toString() : xmlLivre.value("annee")));
        ui->labelLangue->setText(((sql) ? res1.record().value("langue").toString() : xmlLivre.value("langue")));
        ui->labelSousTitres->setText(((sql) ? res1.record().value("sous_titres").toString() : xmlLivre.value("sousTitres")));
        ui->labelNationalite->setText(((sql) ? res1.record().value("pays").toString() : xmlLivre.value("pays")));
        ui->labelDuree->setText(((sql) ? res1.record().value("duree").toString() : xmlLivre.value("duree")));
        ui->labelClassement->setText(((sql) ? res1.record().value("classement").toString() : xmlLivre.value("classement")));
        ui->labelNote->setText(((sql) ? res1.record().value("note").toString() : xmlLivre.value("note"))+"/20");
        ui->plainTextEditSynopsis->insertPlainText(((sql) ? res1.record().value("synopsis").toString() : xmlLivre.value("synopsis")));
        ui->plainTextEditCommentaire->insertPlainText(((sql) ? res1.record().value("commentaire").toString() : xmlLivre.value("commentaire")));
        ui->checkBoxPrete->setChecked((sql) ? res1.record().value("prete").toBool() : ((xmlLivre.value("prete") == "True") ? true : false));
        ui->checkBoxEmpruntable->setChecked((sql) ? res1.record().value("empruntable").toBool() : ((xmlLivre.value("empruntable") == "True") ? true : false));
        ui->checkBoxFichier->setChecked((sql) ? res1.record().value("fichier").toBool() : ((xmlLivre.value("fichier") == "True") ? true : false));
        //TODO chemin FICHIER
        ui->checkBoxVu->setChecked((sql) ? res1.record().value("vu").toBool() : ((xmlLivre.value("vu") == "True") ? true : false));
        //TODO Étiquettes SQL
        if(!sql){
            QString resultat;
            QStringList etiquettes = xmlLivre.value("etiquettes").split(",");
            for(int i=0; i<etiquettes.count(); ++i){
                resultat.append("<a href=\""+etiquettes.at(i)+"\">"+etiquettes.at(i)+"</a> ");
            }
            ui->labelEtiquettesText->setText(resultat);
            connect(ui->labelEtiquettesText, SIGNAL(linkActivated(QString)), this, SLOT(showEtiquette(QString)));
        }

        //Mappeurs pour les boutons
        connect(ui->pushButtonDelete, SIGNAL(clicked()), mapperDelete, SLOT(map()));
        connect(ui->pushButtonEdit, SIGNAL(clicked()), mapperEdit, SLOT(map()));
        mapperDelete->setMapping(ui->pushButtonDelete, ((sql) ? res1.record().value("id").toInt() : xmlLivre.value("id").toInt()));
        mapperEdit->setMapping(ui->pushButtonEdit, ((sql) ? res1.record().value("id").toInt() : xmlLivre.value("id").toInt()));

        //QListWidget pour les acteurs et les genres
        ui->listWidgetActeurs->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        ui->listWidgetActeurs->addItems(((sql) ? res1.record().value("acteurs").toString() : xmlLivre.value("acteurs")).split(","));
        ui->listWidgetGenres->addItems(((sql) ? res1.record().value("genre").toString() : xmlLivre.value("genre")).split(","));
    }
    else{
        //------------------------------
        //LIVRES
        //------------------------------
        ui->labelTitreLivre->setText(((sql) ? res1.record().value("titre").toString() : xmlLivre.value("titre")));
        ui->labelIDLivre->setText(((sql) ? res1.record().value("id").toString() : xmlLivre.value("id")));
        ui->labelImageLivre->setPixmap(ToolsManager::getPixmapFromString(((sql) ? res1.record().value("couverture").toString() : xmlLivre.value("couverture"))));
        ui->labelAuteurLivre->setText(((sql) ? res1.record().value("auteur").toString() : xmlLivre.value("auteur")));
        ui->labelCoAuteurLivre->setText(((sql) ? res1.record().value("coauteur").toString() : xmlLivre.value("coauteur")));
        ui->labelEditeurLivre->setText(((sql) ? res1.record().value("editeur").toString() : xmlLivre.value("editeur")));
        ui->labelAnneeLivre->setText(((sql) ? res1.record().value("annee").toString() : xmlLivre.value("annee")));
        ui->labelPagesLivre->setText(((sql) ? res1.record().value("pages").toString() : xmlLivre.value("pages")));
        ui->labelLangueLivre->setText(((sql) ? res1.record().value("langue").toString() : xmlLivre.value("langue")));
        ui->labelISBNLivre->setText(((sql) ? res1.record().value("isbn").toString() : xmlLivre.value("isbn")));

        ui->labelEditionLivre->setText(((sql) ? res1.record().value("edition").toString() : xmlLivre.value("edition")));
        ui->labelExemplairesLivre->setText(((sql) ? res1.record().value("exemplaires").toString() : xmlLivre.value("exemplaires")));
        ui->labelNoteLivre->setText(((sql) ? res1.record().value("note").toString() : xmlLivre.value("note"))+"/20");
        ui->checkBoxLuLivre->setChecked(((sql) ? res1.record().value("lu").toBool() : ((xmlLivre.value("lu") == "True") ? true : false)));
        ui->checkBoxEmpruntableLivre->setChecked(((sql) ? res1.record().value("empruntable").toBool() : ((xmlLivre.value("empruntable") == "True") ? true : false)));
        ui->checkBoxPreteLivre->setChecked(((sql) ? res1.record().value("prete").toBool() : ((xmlLivre.value("prete") == "True") ? true : false)));
        ui->checkBoxEbookLivre->setChecked(((sql) ? res1.record().value("ebook").toBool() : ((xmlLivre.value("ebook") == "True") ? true : false)));
        ui->labelEmplacementLivre->setText(((sql) ? res1.record().value("emplacement").toString() : xmlLivre.value("emplacement")));
        if((sql && !res1.record().value("ebook").toBool()) || (!sql && (xmlLivre.value("ebook") != "True")))
            ui->labelEmplacementLivre->setHidden(true);
        ui->labelClassementLivre->setText(((sql) ? res1.record().value("classement").toString() : xmlLivre.value("classement")));
        ui->plainTextEditResumeLivre->clear(); ui->plainTextEditResumeLivre->insertPlainText(((sql) ? res1.record().value("synopsis").toString() : xmlLivre.value("synopsis")));
        ui->plainTextEditCommentaireLivre->clear(); ui->plainTextEditCommentaire->insertPlainText(((sql) ? res1.record().value("commentaire").toString() : xmlLivre.value("commentaire")));

        //Mappeurs pour les boutons
        connect(ui->pushButtonDeleteLivre, SIGNAL(clicked()), mapperDelete, SLOT(map()));
        connect(ui->pushButtonEditLivre, SIGNAL(clicked()), mapperEdit, SLOT(map()));
        mapperDelete->setMapping(ui->pushButtonDeleteLivre, ((sql) ? res1.record().value("id").toInt() : xmlLivre.value("id").toInt()));
        mapperEdit->setMapping(ui->pushButtonEditLivre, ((sql) ? res1.record().value("id").toInt() : xmlLivre.value("id").toInt()));
    }
}

//Affiche la liste des livres/films qui contiennent l'étiquette TAG
void DadaBooks::openTagList(QString tag){
    bool resultat = false;
    int id = tag.toInt(&resultat);
    if(resultat){
        this->showEtiquette(id);
    }
}

//Affiche ou cache les auteurs et les genres en cas de prévisualisation dans l'onglet courant
void DadaBooks::expandIntabContent(bool status){
    if(status){
        ui->pushButtonListes->setIcon(QIcon(":/boutons/images/moins.png"));
        ui->listWidgetGenres->setHidden(false);
        ui->listWidgetActeurs->setHidden(false);
    }
    else{
        ui->pushButtonListes->setIcon(QIcon(":/boutons/images/plus.png"));
        ui->listWidgetActeurs->setHidden(true);
        ui->listWidgetGenres->setHidden(true);
    }
}

//Sélection un film/livre non vu/lu au hasard
void DadaBooks::selectRandom(){
    //Détection du type de collection
    bool films = ((QString::compare(insSettingsManager->getSettings(Type).toString(), "films", Qt::CaseInsensitive) != 0) ? false : true);
    bool sql = !insSettingsManager->getSettings(Xml).toBool();
    int id = -1;
    if(sql){
        QSqlQuery res1;
        QString req1;
        if(films){
            req1 = "SELECT id FROM films WHERE vu=0 ORDER BY RAND() LIMIT 1";
        }
        else{
            req1 = "SELECT id FROM livres WHERE lu=0 ORDER BY RAND() LIMIT 1";
        }
        res1 = insSqlManager->query(req1);
        res1.first();
        id = res1.record().value("id").toInt();
    }
    else{ //XML
        QList<QMultiMap<QString, QString> > base = insXmlManager->readBase();
        QList<int> ids;
        for( int i=0; i<base.count(); ++i ){
            if(films){
                if(base.at(i).value("vu") == "False"){
                    ids.append(base.at(i).value("id").toInt());
                }
            }
            else{
                if(base.at(i).value("lu") == "False"){
                    ids.append(base.at(i).value("id").toInt());
                }
            }
        }
        srand(time(NULL));
        if(ids.count() == 0)
            return;
        id = ids.at((rand()%ids.count()));
    }
    if(insSettingsManager->getSettings(OpenInTab).toBool()){
        this->intabPreview(id);
    }
    else{
        this->activatePreview(id);
    }
    return;
}

//Exporte la liste de films en HTML
void DadaBooks::exportAsHTML(){
    QString chemin = insSettingsManager->getSettings(Fichier).toString();
    chemin = chemin.left(chemin.lastIndexOf("/")+1);
    QDir dossierImage; dossierImage.setPath(chemin);
    if(insSettingsManager->getSettings(Xml).toBool()){
        ToolsManager::exportMovieList(insXmlManager->readBase(), dossierImage.path());
    }
    return;
}

void DadaBooks::exportAsPDF(){
    QString chemin = insSettingsManager->getSettings(Fichier).toString();
    chemin = chemin.left(chemin.lastIndexOf("/")+1);
    QDir dossierImage; dossierImage.setPath(chemin);
    if(insSettingsManager->getSettings(Xml).toBool()){
        ToolsManager::exportMovieList(insXmlManager->readBase(), dossierImage.path(), true);
    }
    return;
}
