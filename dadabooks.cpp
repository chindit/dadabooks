#include "dadabooks.h"
#include "ui_dadabooks.h"

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
        else{
            insSqlManager = new SqlManager();
}
    insEditBook = new EditBook;
    insSettingsDialog = new SettingsDialog;

    //Connexion des signaux et des slots
    connect(ui->pushButton_add, SIGNAL(clicked()), insAddBook, SLOT(show()));
    connect(insAddBook, SIGNAL(searchInternet(QString,QString)), this, SLOT(rechercheInternet(QString,QString)));
    connect(insPreviewBook, SIGNAL(bookSelected(QString)), this, SLOT(getBook(QString)));
    connect(ui->tabWidget, SIGNAL(tabCloseRequested(int)), this, SLOT(closeTab(int)));
    connect(insEditBook, SIGNAL(editDone(int)), this, SLOT(updateOnglet(int)));
    connect(insEditBook, SIGNAL(bookAdded()), this, SLOT(setListeLivres()));
    connect(ui->pushButton_options, SIGNAL(clicked()), insSettingsDialog, SLOT(show()));
    connect(ui->actionQuitter, SIGNAL(triggered()), this, SLOT(close()));
    connect(ui->action_propos_de_DadaBooks, SIGNAL(triggered()), this, SLOT(about()));
    connect(ui->action_propos_de_Qt, SIGNAL(triggered()), qApp, SLOT(aboutQt()));
    connect(ui->actionNouveau, SIGNAL(triggered()), this, SLOT(openNewColl()));

    //Et on liste les livres
    this->setListeLivres();
}

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

void DadaBooks::getBook(QString id){
    insPreviewBook->close();
    QMultiMap<QString,QString> livre;
    livre = insSiteManager->getBook(id, nomSiteRecherche);
    insEditBook->updateUi(livre);
    insEditBook->show();
}

//WARNING : ANALYSER
void DadaBooks::updateOnglet(int id){
    if(idOngletEdit >= 0){
        ui->tabWidget->removeTab(idOngletEdit);
        this->activatePreview(id, false, true);
        ui->tabWidget->setCurrentIndex(idOngletEdit);
        idOngletEdit = -1;
    }
    return;
}

void DadaBooks::setListeLivres(){
    QList<QMultiMap<QString, QString> > resultat;
    QSqlQuery res1;
    //XML
    if(insSettingsManager->getSettings(Xml).toBool()){
        insXmlManager = new XmlManager;
        resultat = insXmlManager->readBase();
    }
    else{
        //SQL
        QString req1 = "SELECT livres.id, livres.titre, livres.annee, auteurs.nom, editeurs.nom AS nom_editeur FROM livres LEFT JOIN auteurs ON livres.auteur = auteurs.id LEFT JOIN editeurs ON livres.editeur = editeurs.id ORDER BY livres.id DESC LIMIT 0, 25";
        res1 = insSqlManager->query(req1);
    }

    //Nettoyage & préparation du widget
    ui->tableWidget->clear();
    ui->tableWidget->setColumnCount(5);
    QStringList liste_headers;
    liste_headers << "ID" << "Titre" << "Auteur" << "Éditeur" << "Année";
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
            item2->setText(resultat.at(resultat.size()-1-i).value("auteur"));
            item3->setText(resultat.at(resultat.size()-1-i).value("editeur"));
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
            item2->setText(res1.record().value("nom").toString());
            item3->setText(res1.record().value("nom_editeur").toString());
            item4->setText(res1.record().value("annee").toString());
            ui->tableWidget->setItem(0, 0, item0);
            ui->tableWidget->setItem(0, 1, item1);
            ui->tableWidget->setItem(0, 2, item2);
            ui->tableWidget->setItem(0, 3, item3);
            ui->tableWidget->setItem(0, 4, item4);
        }
        //Et l'accueil
        QString req2 = "SELECT livres.titre FROM livres ORDER BY livres.id";
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

void DadaBooks::preparePreview(){
    //Le but de cette fonction est simplement de convertir l'ID d'index du QTableView en un ID de livre pour appeler ensuite activatePreview()
    int id = 0;
    if(insSettingsManager->getSettings(Xml).toBool()){
        if(!ui->listWidget_accueil->selectedItems().isEmpty())
            id = insXmlManager->getIdByTitle(ui->listWidget_accueil->selectedItems().first()->text());
    }
    else{
        QSqlQuery resultat = insSqlManager->query("SELECT id FROM livres WHERE titre="+ui->listWidget_accueil->currentItem()->text());
        id = resultat.record().value("id").toInt();
    }
    if(id > 0){
        this->activatePreview(id, false, true);
    }
}

void DadaBooks::activatePreview(int id, bool search, bool idOk){
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
    QGridLayout *layout_onglet = new QGridLayout(nv_onglet);
    nv_onglet->setLayout(layout_onglet);
    QString req1 = "SELECT livres.id, livres.titre, livres.ISBN, livres.coauteurs, livres.synopsis, livres.couverture, livres.pages, livres.edition, livres.langue, livres.classement, livres.exemplaires, livres.commentaire, livres.lu, livres.note, livres.empruntable, livres.pret, livres.annee, auteurs.nom, editeurs.nom AS nom_editeur FROM livres LEFT JOIN auteurs ON livres.auteur = auteurs.id LEFT JOIN editeurs ON livres.editeur = editeurs.id WHERE livres.id = ";
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
    isbn = new QLabel("ISBN");
    coauteurs = new QLabel("Co-auteur(s)");
    synopsis = new QLabel("Résumé");
    couverture = new QLabel("Couverture");
    couverture2 = new QLabel();
    QPixmap loader;
    pages = new QLabel("Nombre de pages");
    edition = new QLabel("Édition");
    langue = new QLabel("Langue de publication");
    exemplaires = new QLabel("Nombre d'exemplaires");
    commentaire = new QLabel("Commentaire");
    classement = new QLabel("Classement");
    annee = new QLabel("Année de publication");
    auteur = new QLabel("Auteur");
    editeur = new QLabel("Éditeur");
    note1 = new QLabel("Note");
    ebook2 = new QLabel("Emplacement");
    QCheckBox *empruntable, *prete, *lu, *ebook1;
    empruntable = new QCheckBox("Empruntable");
    empruntable->setEnabled(false);
    prete = new QCheckBox("En prêt");
    prete->setEnabled(false);
    lu = new QCheckBox("Lu");
    lu->setEnabled(false);
    ebook1 = new QCheckBox("Ebook");
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

    QSignalMapper *mapper_delete = new QSignalMapper;
    connect(button_delete, SIGNAL(clicked()), mapper_delete, SLOT(map()));
    QSignalMapper *mapper_edit = new QSignalMapper;
    connect(button_edit, SIGNAL(clicked()), mapper_edit, SLOT(map()));

    if(insSettingsManager->getSettings(Xml).toBool()){
        //XML
        titre_fen = new QLabel(xmlLivre.value("titre"));
        id2 = new QLabel(xmlLivre.value("id"));
        titre2 = new QLabel(xmlLivre.value("titre"));
        isbn2 = new QLabel(xmlLivre.value("isbn"));
        coauteurs2 = new QLabel(xmlLivre.value("coauteurs"));
        QNetworkAccessManager nw_manager;
        QNetworkRequest request(xmlLivre.value("couverture"));
        QNetworkReply *reponse = nw_manager.get(request);
        QEventLoop eventLoop;
        connect(reponse, SIGNAL(finished()), &eventLoop, SLOT(quit()));
        eventLoop.exec();
        QByteArray data = reponse->readAll();
        loader.loadFromData(data);
        int width = loader.width();
        if(width > 150){
            float coef = (float)width / 150;
            int result = loader.width()/coef;
            loader = loader.scaledToWidth(result);
        }
        couverture2->setPixmap(loader);
        pages2 = new QLabel(xmlLivre.value("pages"));
        edition2 = new QLabel(xmlLivre.value("edition"));
        langue2 = new QLabel(xmlLivre.value("langue"));
        exemplaires2 = new QLabel(xmlLivre.value("exemplaires"));
        classement2 = new QLabel(xmlLivre.value("classement"));
        annee2 = new QLabel(xmlLivre.value("annee"));
        auteur2 = new QLabel(xmlLivre.value("auteur"));
        editeur2 = new  QLabel(xmlLivre.value("editeur"));
        note2 = new QLabel(xmlLivre.value("note"));
        ebook3 = new QLabel(xmlLivre.value("emplacement", ""));
        synopsis2->insertPlainText(xmlLivre.value("synopsis"));
        commentaire2->insertPlainText(xmlLivre.value("commentaire"));
        prete->setChecked(((xmlLivre.value("prete") == "True") ? true : false));
        empruntable->setChecked(((xmlLivre.value("empruntable") == "True") ? true : false));
        ebook1->setChecked(((xmlLivre.value("ebook") == "True") ? true : false));
        mapper_delete->setMapping(button_delete, xmlLivre.value("id").toInt());
        mapper_edit->setMapping(button_edit, xmlLivre.value("id").toInt());
    }
    else{
        //SQL
        titre_fen = new QLabel(res1.record().value("titre").toString());
        id2 = new QLabel(res1.record().value("id").toString());
        titre2 = new QLabel(res1.record().value("titre").toString());
        isbn2 = new QLabel(res1.record().value("isbn").toString());
        coauteurs2 = new QLabel(res1.record().value("coauteurs").toString());
        QNetworkAccessManager nw_manager;
        QNetworkRequest request(res1.record().value("couverture").toString());
        QNetworkReply *reponse = nw_manager.get(request);
        QEventLoop eventLoop;
        connect(reponse, SIGNAL(finished()), &eventLoop, SLOT(quit()));
        eventLoop.exec();
        QByteArray data = reponse->readAll();
        loader.loadFromData(data);
        int width = loader.width();
        if(width > 150){
            float coef = (float)width / 150;
            int result = loader.width()/coef;
            loader = loader.scaledToWidth(result);
        }
        couverture2->setPixmap(loader);
        pages2 = new QLabel(res1.record().value("pages").toString());
        edition2 = new QLabel(res1.record().value("edition").toString());
        langue2 = new QLabel(res1.record().value("langue").toString());
        exemplaires2 = new QLabel(res1.record().value("exemplaires").toString());
        classement2 = new QLabel(res1.record().value("classement").toString());
        annee2 = new QLabel(res1.record().value("annee").toString());
        auteur2 = new QLabel(res1.record().value("nom").toString());
        editeur2 = new QLabel(res1.record().value("nom_editeur").toString());
        note2 = new QLabel(res1.record().value("note").toString());
        synopsis2->insertPlainText(res1.record().value("synopsis").toString());
        commentaire2->insertPlainText(res1.record().value("commentaire").toString());
        ebook3 = new QLabel(res1.record().value("emplacement").toString());
        empruntable->setChecked(res1.record().value("empruntable").toBool());
        prete->setChecked(res1.record().value("pret").toBool());
        ebook1->setChecked(res1.record().value("ebook").toBool());
        mapper_delete->setMapping(button_delete, res1.record().value("id").toInt());
        mapper_edit->setMapping(button_edit, res1.record().value("id").toInt());
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

    layout_onglet->addItem(spacer, 11, 1, 1, 3, Qt::AlignHCenter);
    ui->tabWidget->setCurrentWidget(nv_onglet);
    return;
}

void DadaBooks::closeTab(int tab){
    if(ui->tabWidget->tabText(tab) == "Accueil" || ui->tabWidget->tabText(tab) == "Récents"){
        return;
    }
    else{
        ui->tabWidget->removeTab(tab);
    }
}

void DadaBooks::editLivre(int id){
    idOngletEdit = ui->tabWidget->currentIndex();
    QMultiMap<QString, QString> livre;
    QString req1 = "SELECT livres.id, livres.titre, livres.ISBN, livres.coauteurs, livres.synopsis, livres.couverture, livres.pages, livres.edition, livres.langue, livres.classement, livres.exemplaires, livres.commentaire, livres.note, livres.lu, livres.empruntable, livres.pret, livres.ebook, livres.emplacement, livres.annee, auteurs.nom, editeurs.nom AS nom_editeur FROM livres LEFT JOIN auteurs ON livres.auteur = auteurs.id LEFT JOIN editeurs ON livres.editeur = editeurs.id WHERE livres.id = "+QString::number(id)+";";
    if(insSettingsManager->getSettings(Xml).toBool()){
        //XML
        livre = insXmlManager->getBook(id);
        livre.insert("xml", "1");
    }
    else{
        QSqlQuery res1 = insSqlManager->query(req1);
        res1.next();
        livre.insert("couverture", res1.record().value("couverture").toString());
        livre.insert("titre", res1.record().value("titre").toString());
        livre.insert("auteur", res1.record().value("nom").toString());
        livre.insert("editeur", res1.record().value("nom_editeur").toString());
        livre.insert("annee", res1.record().value("annee").toString());
        livre.insert("isbn", res1.record().value("isbn").toString());
        livre.insert("langue", res1.record().value("langue").toString());
        livre.insert("pages", res1.record().value("pages").toString());
        livre.insert("synopsis", res1.record().value("synopsis").toString());
        livre.insert("coauteurs", res1.record().value("coauteurs").toString());
        livre.insert("edition", res1.record().value("edition").toString());
        livre.insert("exemplaires", res1.record().value("exemplaires").toString());
        livre.insert("commentaire", res1.record().value("commentaire").toString());
        livre.insert("classement", res1.record().value("classement").toString());
        livre.insert("empruntable", res1.record().value("empruntable").toString());
        livre.insert("prete", res1.record().value("pret").toString());
        livre.insert("note", res1.record().value("note").toString());
        livre.insert("lu", res1.record().value("lu").toString());
        livre.insert("ebook", res1.record().value("ebook").toString());
        livre.insert("emplacement", res1.record().value("emplacement").toString());
        livre.insert("xml", "0");
    }
    insEditBook->setId(id);
    insEditBook->updateUi(livre);
    insEditBook->show();
    return;
}

void DadaBooks::deleteLivre(int id){
    QString titre;
    if(insSettingsManager->getSettings(Xml).toBool()){
        titre = insXmlManager->getBook(id).value("titre");
    }
    else{
        QString req1 = "SELECT titre FROM livres WHERE id = "+QString::number(id)+";";
        QSqlQuery res1 = insSqlManager->query(req1);
        res1.next();
        titre = res1.record().value("titre").toString();
    }
    int reponse = QMessageBox::question(this, "Supprimer un livre", QString("Êtes-vous sûr de vouloir supprimer <i>%1</i>?").arg(titre), QMessageBox::Yes | QMessageBox::No);
    if(reponse == QMessageBox::Yes){
        if(!insSettingsManager->getSettings(Xml).toBool()){
            QString req1 = QString("DELETE FROM livres WHERE id = %1").arg(id);
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
    return;
}

void DadaBooks::makeSearch(){
    QString recherche = ui->lineEdit_search->text();
    if(recherche.isEmpty() || recherche.isNull()){
        QMessageBox::warning(this, "Chaîne vide", "Aucun terme recherché");
        return;
    }
    QStringList termes = recherche.split(" ");
    QList<QMultiMap<QString, QString> > resultats;
    QSqlQuery res1;
    if(!insSettingsManager->getSettings(Xml).toBool()){
        QString requete = "SELECT livres.id, livres.titre, livres.annee, auteurs.nom, editeurs.nom AS nom_editeur FROM livres LEFT JOIN auteurs ON livres.auteur = auteurs.id LEFT JOIN editeurs ON livres.editeur = editeurs.id WHERE";
        QString temp;
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
    liste_headers << "ID" << "Titre" << "Auteur" << "Éditeur" << "Année";
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
            item2->setText(res1.record().value("nom").toString());
            item3->setText(res1.record().value("nom_editeur").toString());
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

void DadaBooks::about(){
    QString string_about = ("<h2>À propos de DadaBooks</h2><br><b>Dévoloppé par</b> : David Lumaye<br><b>Version</b> : ")+QString(VERSION)+tr("<br><b>Courriel</b>:<a href='mailto:d.lumaye.aro-base.mailtunnel.eu'>d.lumaye.aro-base.mailtunnel.eu</a><br><b>Distribué sous license</b> : <a href='http://www.gnu.org/licenses/gpl-3.0.fr.html'>GPL 3</a>");
    QMessageBox::about(this, tr("À propos de DadaBooks"), string_about);
    return;
}

void DadaBooks::openNewColl(){
    //Pour créer une nouvelle collection, on vire la référence de l'ancienne
    insSettingsManager->setSettings(Fichier, "");
    //Et on rapelle les fonctions d'initialisation
    FirstLaunch *insFirstLaunch = new FirstLaunch;
    insFirstLaunch->exec();
    delete insFirstLaunch;

    //On recharge les paramètres
    insSettingsManager->loadSettings();
    insSettingsManager->setSettings(Initialized, true);
    if(insSettingsManager->getSettings(Fichier).toString().isEmpty() && !insSettingsManager->getSettings(MariaDB).toBool()){
        this->openFile();
    }
    return;
}
