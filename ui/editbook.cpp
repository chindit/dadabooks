#include "editbook.h"

/**
 * Constructor
 * @brief EditBook::EditBook
 * @param storage
 * @param logger
 * @param parent
 * @param collection
 */
EditBook::EditBook(Storage *storage, Logger *logger, QWidget *parent, CollectionStorageSettings collection) : QDialog(parent), ui(new Ui::EditBook){
    this->storage = storage;
    this->logger = logger;
    this->parent = parent;
    this->collection = collection;
    this->entity = new class Movie(); // Movie by default, but it shouldn't be accessed as it.
    this->coverPicture = QPixmap();
    this->networkManager = new NetworkManager(this->logger, this);

    this->idEdit = 0;

    ui->setupUi(this);

    this->setCollectionType(collection.type);
}

/**
 * Constructor overload
 * @brief EditBook::EditBook
 * @param storage
 * @param logger
 * @param parent
 * @param collection
 * @param entity
 */
EditBook::EditBook(Storage *storage, Logger *logger, QWidget *parent, CollectionStorageSettings collection, BaseEntity *entity)
{
    EditBook(storage, logger, parent, collection);
    this->setEntity(entity);
}

/**
 * Destructor
 * @brief EditBook::~EditBook
 */
EditBook::~EditBook(){
    delete ui;
    delete networkManager;
}

/**
 * Connect elements for the UI and network
 * @brief EditBook::setConnectors
 */
void EditBook::setConnectors()
{
    connect(this->networkManager, SIGNAL(downloadAborted()), this, SLOT(pictureHydratorCallback()));
    connect(this->networkManager, SIGNAL(downloadFailed()), this, SLOT(pictureHydratorCallback()));
    connect(this->networkManager, SIGNAL(downloadFinished()), this, SLOT(pictureHydratorCallback()));
}

/**
 * Set type of collection
 * @brief EditBook::setCollectionType
 * @param type
 */
void EditBook::setCollectionType(Collection type)
{
    if (type == Book) {
        this->ui->stackedWidget->setCurrentIndex(0);
    } else if (type == Movie) {
        this->ui->stackedWidget->setCurrentIndex(1);
    } else {
        QMessageBox::warning(this, tr("Unsupported collection type"), tr("Unsupported collection type"));
        this->logger->error("Unknown collection type cannot be added");
        this->close();
    }
}

/**
 * Hydrate dialog with entity data
 * @brief EditBook::setEntity
 * @param entity
 */
void EditBook::setEntity(BaseEntity *entity)
{
    this->entity = entity;
    if (this->collection.type == Movie) {
        try {
            class Movie *movie = dynamic_cast<class Movie*>(entity);
            this->hydrateMovie(movie);
        } catch (const std::exception& e) {
            QMap<QString, QString> context;
            context.insert("error", QString(e.what()));
            this->logger->error("Unable to cast Entity to Movie.", context);
        }
    }
}

void EditBook::hydrateMovie(class Movie *movie)
{
    this->clearUI();
    this->idEdit = movie->getId();

    // Hydrating plain-text
    this->ui->lineEditTitreFilm->setText(movie->getTitle());
    this->ui->lineEditTitreOriginalFilm->setText(movie->getOriginalTitle());
    this->ui->lineEditDirecteurFilm->setText(movie->getDirector());
    this->ui->plainTextEditSynopsisFilm->setPlainText(movie->getSynopsis());
    this->ui->spinBoxAnneeFilm->setValue(movie->getYear().year());
    this->ui->spinBoxDureeFilm->setValue(movie->getDuration());
    this->ui->lineEditNationaliteFilm->setText(movie->getCountries().join(','));
    this->ui->lineEditLangueFilm->setText(movie->getLanguages().join(','));
    this->ui->lineEditSousTitresFilm->setText(movie->getSubtitles().join(','));
    this->ui->labelImageFilm->setText(movie->getCover());
    this->ui->lineEditClassementFilm->setText(movie->getClassification());
    this->ui->horizontalSliderNoteFilm->setValue(movie->getNotation());
    this->ui->labelNoteFilm->setText(QString::number(movie->getNotation()));
    this->ui->plainTextEditCommentaireFilm->setPlainText(movie->getComment());

    // Hydrating checkboxes
    ui->checkBoxEmpruntableFilm->setChecked(movie->isBorrowable());
    ui->checkBoxPreteFilm->setChecked(movie->isBorrowed());
    ui->checkBoxVuFilm->setChecked(movie->isSeen());
    ui->checkBoxFichierFilm->setChecked(movie->hasFile());
    if (movie->hasFile()) {
        ui->labelCheminFilm->setText(movie->getFile());
        ui->pushButtonEmplacementFilm->setEnabled(true);
    }

    // Hydrating QListWidget
    for (QString actor : movie->getActors()) {
        ui->listWidgetActeursFilm->addItem(actor);
        ui->listWidgetActeursFilm->setCurrentRow(ui->listWidgetActeursFilm->count()-1);
        ui->listWidgetActeursFilm->currentItem()->setFlags(Qt::ItemIsEditable | Qt::ItemIsSelectable | Qt::ItemIsEnabled);
    }
    for (QString gender : movie->getGenders()) {
        ui->listWidgetGenreFilm->addItem(gender);
        ui->listWidgetGenreFilm->setCurrentRow(ui->listWidgetGenreFilm->count()-1);
        ui->listWidgetGenreFilm->currentItem()->setFlags(Qt::ItemIsEditable | Qt::ItemIsSelectable | Qt::ItemIsEnabled);
    }

    QStringList availableLabels = this->storage->getAvailableLabels();
    // Removing selected labels from available list
    for(QString label : movie->getLabels()) {
        if (availableLabels.contains(label)) {
            availableLabels.removeOne(label);
        } else {
            this->storage->addLabel(label);
        }
    }

    ui->listWidgetEtiquettesDispoFilm->insertItems(0, availableLabels);
    ui->listWidgetEtiquettesFilmFilm->insertItems(0, movie->getLabels());
}

/**
 * Download a picture in background
 * @brief EditBook::hydratePicture
 * @param picture
 */
void EditBook::hydratePicture(QString picture)
{
    // Hydrating cover picture
    QPixmap image;
    if (StringTools::isUrl(picture)) {
        // Calling non thread-blocking download
        this->networkManager->download(picture);
    } else if (StringTools::isFile(picture)) {
        image.load(picture);
    } else {
        QMap<QString, QString> context;
        context.insert("file", picture);
        context.insert("movieTitle", this->entity->getTitle());
        context.insert("id", QString::number(this->entity->getId()));
        this->logger->info("Picture cover not found", context);
        // TODO Load default cover
    }
}

/**
 * In case of a download, we use this callback to not block main thread
 * @brief EditBook::pictureHydratorCallback
 */
void EditBook::pictureHydratorCallback()
{
    QByteArray response = this->networkManager->getFile();
    if (response.size() > 0) {
        bool imageLoaded = this->coverPicture.loadFromData(response);
        if (!imageLoaded) {
            // TODO Load default cover
        } else {
            int width = this->coverPicture.width();
            if(width > 150){
                float coef = (float)width / 150;
                int result = this->coverPicture.width()/coef;
                this->coverPicture = this->coverPicture.scaledToWidth(result);
            }
        }
    }
}

/**
 * Remove previous data form UI
 * @brief EditBook::clearUI
 */
void EditBook::clearUI()
{
    // Clearing movie
    this->ui->listWidgetActeursFilm->clear();
    this->ui->listWidgetEtiquettesDispoFilm->clear();
    this->ui->listWidgetEtiquettesDispoLivre->clear();
    this->ui->listWidgetEtiquettesFilmFilm->clear();
    this->ui->listWidgetEtiquettesLivreLivre->clear();
    this->ui->listWidgetGenreFilm->clear();
    this->ui->plainTextEditSynopsisFilm->clear();
    this->ui->plainTextEditCommentaireFilm->clear();
}

/**
 * Show cover picture in a dedicated dialog
 * @brief EditBook::showPicture
 */
void EditBook::showPicture()
{
    if (this->coverPicture.isNull()) {
        return;
    }
    QDialog *preview = new QDialog(this);
    preview->setWindowTitle(tr("Cover picture"));
    preview->setWindowModality(Qt::WindowModal);
    QVBoxLayout *layout = new QVBoxLayout;
    QPushButton *close = new QPushButton(tr("Close"));
    QLabel *picture = new QLabel;
    picture->setPixmap(this->coverPicture);
    layout->addWidget(picture);
    layout->addWidget(close);
    preview->setLayout(layout);
    connect(close, SIGNAL(clicked()), preview, SLOT(close()));
    preview->exec();

    delete picture;
    delete close;
    delete layout;
    delete preview;
}

//Envoie le signal d'édition annulée
void EditBook::reject(){
    emit editCanceled();
    this->hide();
    this->close();
    return;
}

//Enregistre l'item en cours d'édition ou le met à jour
void EditBook::accept(){
    //---------------------------
    //LIVRES
    //---------------------------
    if(QString::compare(insSettingsManager->getSettings(Type).toString(), "films", Qt::CaseInsensitive) != 0){
        /*
         * Dans le cas où il s'agit d'une base SQL ET que les plugins auteurs et éditeurs sont activés, on vérifie que
         * les champs correspondants sont bien remplis.
         */
        int id_auteur = 0, id_editeur = 0;
        //1)Vérification de la présence de l'auteur
        if(ui->comboBoxAuteurLivre->currentText().isEmpty() && !insSettingsManager->getSettings(Xml).toBool()){
            if(insSettingsManager->getSettings(AutoAuteur).toBool()){
                id_auteur = insAddAuteur->setAuteur(ui->lineEditAuteurLivre->text());
            }
            else{
                QMessageBox::warning(this, "Auteur manquant", "L'auteur de ce livre n'est pas renseigné.  Veuillez créer une nouvelle entrée");
                return;
            }
        }
        //2)Vérification de la présence de l'éditeur
        if(ui->comboBoxEditeurLivre->currentText().isEmpty() && !insSettingsManager->getSettings(Xml).toBool()){
            if(insSettingsManager->getSettings(AutoEditeur).toBool()){
                id_editeur = insAddEditeur->setEditeur(ui->lineEditEditeurLivre->text());
            }
            else{
                QMessageBox::warning(this, "Éditeur manquant", "L'éditeur de ce livre n'est pas renseigné.  Veuillez créer une nouvelle entrée");
                return;
            }
        }
        //3)Vérification de l'existence de l'auteur et de l'éditeur dans la base de données
        if(!insSettingsManager->getSettings(Xml).toBool()){
            if(id_auteur <= 0)
                id_auteur=this->getId(false);
            if(id_editeur <= 0)
                id_editeur=this->getId(true);
            if(id_auteur <= 0 || id_editeur <= 0){
                QMessageBox::warning(this, "Erreur étrange", "L'auteur ou l'éditeur n'ont pu être trouvés dans la base de données.  Essayez en ré-insérant le nom ou en redémarrant le programme");
                return;
            }
        }
        //4)Vérification du titre
        if(ui->lineEditTitreLivre->text().isEmpty()){
            QMessageBox::critical(this, "Impossible d'ajouter le livre", "Nous sommes désolé mais il est impossible d'ajouter un livre qui ne possède pas de titre");
            return;
        }

        /*
         * Préparation des variables
         * Récupération et traitement des éléments insérés
         */
        int annee, pages, edition, exemplaires, empruntable, pret, lu, note, ebook;
        annee = ui->spinBoxAnneeLivre->value();
        pages = ui->spinBoxPagesLivre->value();
        edition = ui->spinBoxEditionLivre->value();
        exemplaires = ui->spinBoxExemplairesLivre->value();
        empruntable = (ui->checkBoxEmpruntableLivre->isChecked()) ? 1 : 0 ;
        pret = (ui->checkBoxPretLivre->isChecked()) ? 1 : 0 ;
        lu = (ui->checkBoxLuLivre->isChecked()) ? 1 : 0;
        ebook = (ui->checkBoxEbookLivre->isChecked() ? 1 : 0);
        note = ui->horizontalSliderNoteLivre->value();
        QString req1;
        int idLivre = -1;

        //Pour rappel, l'image est AFFICHÉE dans un QLabel et son URL est INDIQUÉE dans un autre.
        //On récupère bien entendu l'URL
        QString nomImage = ui->labelImageLivre->text();
        //Stockage de l'image en local en cas de besoin
        if(!insSettingsManager->getSettings(MariaDB).toBool() && insSettingsManager->getSettings(DownPics).toBool() && nomImage.contains("http")){
            //On vérifie que le dossier existe
            QString chemin = insSettingsManager->getSettings(Fichier).toString();
            chemin = chemin.left(chemin.lastIndexOf("/")+1);
            QDir dossierImage; dossierImage.setPath(chemin+"Images");
            nomImage = ToolsManager::downloadFile(ui->labelImageLivre->text(), dossierImage);
        }

        if(!insSettingsManager->getSettings(Xml).toBool()){
            if(idEdit == 0){ //Si pas d'édition->INSERT
                req1 = "INSERT INTO livres(titre, ISBN, auteur, coauteurs, synopsis, couverture, editeur, annee, pages, edition, langue, classement, exemplaires, commentaire, lu, note, empruntable, prete, ebook, emplacement) VALUES(\""+ToolsManager::guillemets(ui->lineEditTitreLivre->text())+"\", \""+ui->lineEditISBNLivre->text()+"\", "+QString::number(id_auteur)+", \""+ToolsManager::guillemets(ui->lineEditCoAuteurLivre->text())+"\", \""+ToolsManager::guillemets(ui->plainTextEditSynopsisLivre->toPlainText())+"\", \""+ui->labelImageLivre->text()+"\", "+QString::number(id_editeur)+", '"+QString::number(annee)+"', "+QString::number(pages)+", "+QString::number(edition)+", \""+ToolsManager::guillemets(ui->lineEditLangueLivre->text())+"\", \""+ToolsManager::guillemets(ui->lineEditClassementLivre->text())+"\", "+QString::number(exemplaires)+", \""+ToolsManager::guillemets(ui->plainTextEditCommentaireLivre->toPlainText())+"\", "+QString::number(lu)+", "+QString::number(note)+", "+QString::number(empruntable)+", "+QString::number(pret)+", "+QString::number(ebook)+", \""+ui->labelCheminLivre->text()+"\");";
            }
            else{
                req1 = "UPDATE livres SET titre = \""+ToolsManager::guillemets(ui->lineEditTitreLivre->text())+"\", ISBN = \""+ui->lineEditISBNLivre->text()+"\", auteur = "+QString::number(id_auteur)+", coauteurs = \""+ToolsManager::guillemets(ui->lineEditCoAuteurLivre->text())+"\", synopsis = \""+ToolsManager::guillemets(ui->plainTextEditSynopsisLivre->toPlainText())+"\", couverture = \""+nomImage+"\", editeur = "+QString::number(id_editeur)+", annee = '"+QString::number(annee)+"', pages = "+QString::number(pages)+", edition = "+QString::number(edition)+", langue = \""+ToolsManager::guillemets(ui->lineEditLangueLivre->text())+"\", classement = \""+ToolsManager::guillemets(ui->lineEditClassementLivre->text())+"\", exemplaires = "+QString::number(exemplaires)+", commentaire = \""+ToolsManager::guillemets(ui->plainTextEditCommentaireLivre->toPlainText())+"\", lu = "+QString::number(lu)+", note = "+QString::number(note)+", empruntable = "+QString::number(empruntable)+", prete = "+QString::number(pret)+", ebook = "+QString::number(ebook)+", emplacement = \""+ui->labelCheminLivre->text()+"\" WHERE id="+QString::number(idEdit)+";";
            }

            QSqlQuery res1 = insSql->query(req1);
            idLivre = (idEdit == 0) ? res1.lastInsertId().toInt() : idEdit;

            QStringList etiquettes;
            for(int i=0; i<ui->listWidgetEtiquettesLivreLivre->count(); ++i){
                etiquettes.append(ui->listWidgetEtiquettesLivreLivre->item(i)->text());
            }
            if(ui->listWidgetEtiquettesLivreLivre->count() > 0){
                int id = -1;
                //On vire les vieilles étiquettes
                insSql->query("DELETE FROM liste_etiquettes WHERE id_livre="+QString::number(idLivre));
                for(int i=0; i<etiquettes.size(); i++){
                    req1 = "SELECT id FROM etiquettes WHERE nom=\""+etiquettes.at(i).trimmed().trimmed()+"\"";
                    res1 = insSql->query(req1);
                    if(res1.size() < 1){
                        req1 = "INSERT INTO etiquettes(nom) VALUES(\""+etiquettes.at(i).trimmed()+"\")";
                        res1 = insSql->query(req1);
                        id = res1.lastInsertId().toInt();
                    }
                    else{
                        //On a déjà une id
                        res1.first();
                        id = res1.record().value("id").toInt();
                    }
                    //On insère le lien dans la BDD
                    req1 = "INSERT INTO liste_etiquettes(id_livre, id_etiquette) VALUES("+QString::number(idLivre)+", "+QString::number(id)+");";
                    res1 = insSql->query(req1);
                }
            }

        }
        else{
            /*
             * Pour le XML, il suffit de remplir un QMultiMap.  Le plugin XML se chargera de tout
             */
            QString etiquettes;
            for(int i=0; i<ui->listWidgetEtiquettesLivreLivre->count(); ++i){
                etiquettes.append(ui->listWidgetEtiquettesLivreLivre->item(i)->text());
                if(i < (ui->listWidgetEtiquettesLivreLivre->count()-1))
                    etiquettes.append(",");
            }

            QMultiMap<QString, QString> livre;
            livre.insert("titre", ui->lineEditTitreLivre->text());
            livre.insert("isbn", ui->lineEditISBNLivre->text());
            livre.insert("auteur", ui->lineEditAuteurLivre->text());
            livre.insert("editeur", ui->lineEditEditeurLivre->text());
            livre.insert("coauteur", ui->lineEditCoAuteurLivre->text());
            livre.insert("couverture", nomImage);
            livre.insert("pages", QString::number(ui->spinBoxPagesLivre->value()));
            livre.insert("edition", QString::number(ui->spinBoxEditionLivre->value()));
            livre.insert("langue", ui->lineEditLangueLivre->text());
            livre.insert("exemplaires", QString::number(ui->spinBoxExemplairesLivre->value()));
            livre.insert("classement", ui->lineEditClassementLivre->text());
            livre.insert("annee", QString::number(ui->spinBoxAnneeLivre->value()));
            livre.insert("etiquettes", etiquettes);
            livre.insert("synopsis", ui->plainTextEditSynopsisLivre->toPlainText());
            livre.insert("commentaire", ui->plainTextEditCommentaireLivre->toPlainText());
            livre.insert("note", QString::number(ui->horizontalSliderNoteLivre->value()));
            livre.insert("empruntable", ((ui->checkBoxEmpruntableLivre->isChecked()) ? "True" : "False"));
            livre.insert("prete", ((ui->checkBoxPretLivre->isChecked()) ? "True" : "False"));
            livre.insert("lu", ((ui->checkBoxLuLivre->isChecked()) ? "True" : "False"));
            livre.insert("ebook", ((ui->checkBoxEbookLivre->isChecked()) ? "True" : "False"));
            if(ui->checkBoxEbookLivre->isChecked())
                livre.insert("emplacement", ui->labelCheminLivre->text());

            if(idEdit == 0){
                insXml->addBook(livre);
            }
            else{
                //La mise à jour est simple: on vire le vieil item et on insère le nouveau
                livre.insert("id", QString::number(idEdit));
                insXml->deleteBook(idEdit);
                insXml->addBook(livre);
            }
        }
        if(idEdit > 0){
            /*
             * Émission du signal d'édition
             * Ce signal permet à l'élément de se rafraichir dans l'onglet ouvert
             */
            emit editDone(idEdit);
            idEdit = 0;
        }
        else{
            /*
             * Émission du signal d'ajout terminé
             * Dans ce cas, la collection sera réactualisée pour afficher immédiatement le nouvel élément
             */
            emit bookAdded();
        }
    }
    else{
        //---------------------------
        //FILMS
        //---------------------------

        //1)Vérification du titre
        if(ui->lineEditTitreFilm->text().isEmpty()){
            QMessageBox::warning(this, "Pas de titre", "Nous sommes désolé mais il est impossible d'ajouter un film qui ne possède pas de titre");
            return;
        }
        if(ui->labelImageFilm->text().isEmpty()){
            int reponse = QMessageBox::warning(this, "Pas de jaquette", "Le film ne possède pas de jaquette.  Êtes-vous sûr de vouloir continuer?", QMessageBox::Yes | QMessageBox::No);
            if(reponse == QMessageBox::No){
                return;
            }
        }

        //Téléchargement de l'image si besoin
        QString nomImage = ui->labelImageFilm->text();
        if(!insSettingsManager->getSettings(MariaDB).toBool() && insSettingsManager->getSettings(DownPics).toBool() && nomImage.contains("http")){
            //On vérifie que le dossier existe
            //QString chemin = insSettingsManager->getSettings(Fichier).toString();
            //chemin = chemin.left(chemin.lastIndexOf("/")+1);
            QFileInfo xmlPath(insSettingsManager->getSettings(Fichier).toString());
            QDir dossierImage; dossierImage.setPath(xmlPath.absolutePath()+"/Images");
            if(!dossierImage.exists()){
                if(!dossierImage.mkdir(dossierImage.path())){
                    QMessageBox::critical(this, "Impossible de télécharger les images", "Impossible de créer le dossier «"+dossierImage.absolutePath()+"».  Veuillez le créer manuellement.");
                    return;
                }
            }
            nomImage = ToolsManager::downloadFile(ui->labelImageFilm->text(), dossierImage);
        }

        //Préparation des variables de données pour écriture
        QString etiquettes;
        for(int i=0; i<ui->listWidgetEtiquettesFilmFilm->count(); ++i){
            etiquettes.append(ui->listWidgetEtiquettesFilmFilm->item(i)->text());
            if(i < (ui->listWidgetEtiquettesFilmFilm->count()-1))
                etiquettes.append(",");
        }
        QString acteurs;
        for(int i=0; i<ui->listWidgetActeursFilm->count(); ++i){
            if(!ui->listWidgetActeursFilm->item(i)->text().isEmpty()){
                acteurs.append(ui->listWidgetActeursFilm->item(i)->text());
                if(i < (ui->listWidgetActeursFilm->count()-1))
                    acteurs.append(",");
            }
        }
        QString genres;
        for(int i=0; i<ui->listWidgetGenreFilm->count(); ++i){
            if(!ui->listWidgetGenreFilm->item(i)->text().isEmpty()){
                genres.append(ui->listWidgetGenreFilm->item(i)->text());
                if(i < (ui->listWidgetGenreFilm->count()-1))
                    genres.append(",");
            }
        }

        //2)On stocke tout en SQL ou XML
        if(!insSettingsManager->getSettings(Xml).toBool()){
            int idFilm = -1;
            QString req1;
            if(idEdit == 0){ //Si pas d'édition->INSERT
                req1 = "INSERT INTO films(id, titre, titre_original, directeur, acteurs, synopsis, annee, duree, genre, pays, langue, classement, sous_titres, commentaire, note, jaquette, empruntable, prete, vu, fichier, emplacement, qualite) VALUES(NULL, \""+ToolsManager::guillemets(ui->lineEditTitreFilm->text())+"\", \""+ToolsManager::guillemets(ui->lineEditTitreOriginalFilm->text())+"\", \""+ToolsManager::guillemets(ui->lineEditDirecteurFilm->text())+"\", \""+ToolsManager::guillemets(acteurs)+"\", \""+ToolsManager::guillemets(ui->plainTextEditSynopsisFilm->toPlainText())+"\", "+QString::number(ui->spinBoxAnneeFilm->value())+", "+QString::number(ui->spinBoxDureeFilm->value())+", \""+ToolsManager::guillemets(genres)+"\", \""+ToolsManager::guillemets(ui->lineEditNationaliteFilm->text())+"\", \""+ToolsManager::guillemets(ui->lineEditLangueFilm->text())+"\", \""+ToolsManager::guillemets(ui->lineEditClassementFilm->text())+"\", \""+ToolsManager::guillemets(ui->lineEditSousTitresFilm->text())+"\", \""+ToolsManager::guillemets(ui->plainTextEditCommentaireFilm->toPlainText())+"\", "+QString::number(ui->horizontalSliderNoteFilm->value())+", \""+nomImage+"\", "+((ui->checkBoxEmpruntableFilm->isChecked()) ? QString::number(1) : QString::number(0))+", "+((ui->checkBoxPreteFilm->isChecked()) ? QString::number(1) : QString::number(0))+", "+((ui->checkBoxVuFilm->isChecked()) ? QString::number(1) : QString::number(0))+", "+((ui->checkBoxFichierFilm->isChecked()) ? QString::number(1) : QString::number(0))+", \""+ToolsManager::guillemets(ui->labelCheminFilm->text())+"\", \"\")";
            }
            else{
                req1 = "UPDATE films SET titre=\""+ToolsManager::guillemets(ui->lineEditTitreFilm->text())+"\", titre_original=\""+ToolsManager::guillemets(ui->lineEditTitreOriginalFilm->text())+"\", directeur=\""+ToolsManager::guillemets(ui->lineEditDirecteurFilm->text())+"\", acteurs=\""+ToolsManager::guillemets(acteurs)+"\", synopsis=\""+ToolsManager::guillemets(ui->plainTextEditSynopsisFilm->toPlainText())+"\", annee="+QString::number(ui->spinBoxAnneeFilm->value())+", duree="+QString::number(ui->spinBoxDureeFilm->value())+", genre=\""+ToolsManager::guillemets(genres)+"\", pays=\""+ToolsManager::guillemets(ui->lineEditNationaliteFilm->text())+"\", langue=\""+ToolsManager::guillemets(ui->lineEditLangueFilm->text())+"\", classement=\""+ToolsManager::guillemets(ui->lineEditClassementFilm->text())+"\", sous_titres=\""+ToolsManager::guillemets(ui->lineEditSousTitresFilm->text())+"\", commentaire=\""+ToolsManager::guillemets(ui->plainTextEditCommentaireFilm->toPlainText())+"\", note="+QString::number(ui->horizontalSliderNoteFilm->value())+", jaquette=\""+nomImage+"\", empruntable="+((ui->checkBoxEmpruntableFilm->isChecked()) ? QString::number(1) : QString::number(0))+", prete="+((ui->checkBoxPreteFilm->isChecked()) ? QString::number(1) : QString::number(0))+", vu="+((ui->checkBoxVuFilm->isChecked()) ? QString::number(1) : QString::number(0))+", fichier="+((ui->checkBoxFichierFilm->isChecked()) ? QString::number(1) : QString::number(0))+", emplacement=\""+ToolsManager::guillemets(ui->labelCheminFilm->text())+"\" WHERE id="+QString::number(idEdit)+";";
            }
            QSqlQuery res1 = insSql->query(req1);
            idFilm = (idEdit == 0) ? res1.lastInsertId().toInt() : idEdit;
            if(ui->listWidgetEtiquettesFilmFilm->count() > 0){
                int id = -1;
                //On vire les vieilles étiquettes
                insSql->query("DELETE FROM liste_etiquettes WHERE id_livre="+QString::number(idFilm));
                for(int i=0; i<ui->listWidgetEtiquettesFilmFilm->count(); i++){
                    req1 = "SELECT id FROM etiquettes WHERE nom=\""+ToolsManager::guillemets(ui->listWidgetEtiquettesFilmFilm->item(i)->text().trimmed())+"\"";
                    res1 = insSql->query(req1); res1.first();
                    if(res1.size() < 1){
                        req1 = "INSERT INTO etiquettes(nom) VALUES(\""+ToolsManager::guillemets(ui->listWidgetEtiquettesFilmFilm->item(i)->text().trimmed())+"\")";
                        res1 = insSql->query(req1);
                        id = res1.lastInsertId().toInt();
                    }
                    else{
                        //On a déjà une id
                        res1.first();
                        id = res1.record().value("id").toInt();
                    }
                    //On insère le lien dans la BDD
                    req1 = "INSERT INTO liste_etiquettes(id_livre, id_etiquette) VALUES("+QString::number(idFilm)+", "+QString::number(id)+");";
                    res1 = insSql->query(req1);
                }
            }

        }
        else{
            //XML
            QMultiMap<QString, QString> film;
            film.insert("titre", ui->lineEditTitreFilm->text());
            film.insert("titreOriginal", ui->lineEditTitreOriginalFilm->text());
            film.insert("directeur", ui->lineEditDirecteurFilm->text());
            film.insert("acteurs", acteurs);
            film.insert("synopsis", ui->plainTextEditSynopsisFilm->toPlainText());
            film.insert("annee", QString::number(ui->spinBoxAnneeFilm->value()));
            film.insert("duree", QString::number(ui->spinBoxDureeFilm->value()));
            film.insert("genre", genres);
            film.insert("pays", ui->lineEditNationaliteFilm->text());
            film.insert("langue", ui->lineEditLangueFilm->text());
            film.insert("classement", ui->lineEditClassementFilm->text());
            film.insert("sousTitres", ui->lineEditSousTitresFilm->text());
            film.insert("commentaire", ui->plainTextEditCommentaireFilm->toPlainText());
            film.insert("note", QString::number(ui->horizontalSliderNoteFilm->value()));
            film.insert("couverture", nomImage);
            film.insert("etiquettes", etiquettes);
            film.insert("empruntable", ((ui->checkBoxEmpruntableFilm->isChecked()) ? "True" : "False"));
            film.insert("prete", ((ui->checkBoxPreteFilm->isChecked()) ? "True" : "False"));
            film.insert("vu", ((ui->checkBoxVuFilm->isChecked()) ? "True" : "False"));
            film.insert("fichier", ((ui->checkBoxFichierFilm->isChecked()) ? "True" : "False"));
            if(ui->checkBoxFichierFilm->isChecked())
                film.insert("emplacement", ui->labelCheminFilm->text());
            //On ajoute si pas d'édition
            if(idEdit == 0){
                insXml->addBook(film);
            }
            else{
                //Sinon on supprime le précédent et on ajoute le nouveau
                film.insert("id", QString::number(idEdit));
                insXml->deleteBook(idEdit);
                insXml->addBook(film);
            }
        }

        if(idEdit > 0){
            emit editDone(idEdit);
            idEdit = 0;
        }
        else{
            emit bookAdded();
        }
    }
    this->hide();
    this->close();
    return;
}

//Dans le cas où il ne s'agit pas d'un remplissage automatique, on affiche quelques outils pour l'édition manuelle
void EditBook::setManual(){
    this->clearAll();
    QStringList etiquettes;
    if(!insSettingsManager->getSettings(Xml).toBool()){
        QSqlQuery res1 = insSql->query("SELECT nom FROM etiquettes");
        while(res1.next())
            etiquettes.append(res1.record().value("nom").toString());
    }

    if(ui->stackedWidget->currentIndex() == 1){
        if(insSettingsManager->getSettings(Xml).toBool())
            ui->listWidgetEtiquettesDispoFilm->addItems(insXml->getListEtiquettes());
        else
            ui->listWidgetEtiquettesDispoFilm->addItems(etiquettes);
        if(insSettingsManager->getSettings(Empruntable).toBool())
            ui->checkBoxEmpruntableFilm->setChecked(true);
    }
    else{
        if(insSettingsManager->getSettings(Xml).toBool())
            ui->listWidgetEtiquettesDispoLivre->addItems(insXml->getListEtiquettes());
        else
            ui->listWidgetEtiquettesDispoLivre->addItems(etiquettes);
        if(insSettingsManager->getSettings(Empruntable).toBool())
            ui->checkBoxEmpruntableLivre->setChecked(true);
    }
}

//Upload une image locale comme jaquette
void EditBook::uploadImage(){
    ImageUploadDialog insIUD;
    insIUD.exec();
    QString image = insIUD.getImage();
    if(ui->stackedWidget->currentIndex() == 0)
        ui->labelImageLivre->setText(image);
    else
        ui->labelImageFilm->setText(image);
    return;
}

//Met à jour l'UI avec le tableau fourni en paramètre
void EditBook::updateUi(QMultiMap<QString, QString> livre){
    //On détecte s'il s'agit d'un livre ou d'un film
    bool films = ((QString::compare(insSettingsManager->getSettings(Type).toString(), "films", Qt::CaseInsensitive) != 0) ? false : true);

    if(!films){
        ui->stackedWidget->setCurrentIndex(0);
        ui->lineEditTitreLivre->setText(livre.value("titre"));
        ui->lineEditAuteurLivre->setText(livre.value("auteur"));
        ui->lineEditEditeurLivre->setText(livre.value("editeur"));
        ui->spinBoxAnneeLivre->setValue(livre.value("annee").toInt());
        ui->lineEditISBNLivre->setText(livre.value("isbn"));
        ui->lineEditLangueLivre->setText(livre.value("langue"));
        ui->spinBoxPagesLivre->setValue(livre.value("pages").toInt());
        ui->plainTextEditSynopsisLivre->clear();
        ui->plainTextEditSynopsisLivre->insertPlainText(livre.value("synopsis"));
        ui->lineEditCoAuteurLivre->setText(livre.value("coauteurs", ""));
        ui->spinBoxEditionLivre->setValue(livre.value("edition", "1").toInt());
        ui->spinBoxExemplairesLivre->setValue(livre.value("exemplaires", "1").toInt());
        ui->plainTextEditCommentaireLivre->clear();
        ui->plainTextEditCommentaireLivre->insertPlainText(livre.value("commentaire", ""));
        ui->lineEditClassementLivre->setText(livre.value("classement", ""));
        if(livre.value("empruntable", "0").toInt() == 1)
            ui->checkBoxEmpruntableLivre->setChecked(true);
        if(livre.value("prete", "0").toInt() == 1)
            ui->checkBoxPretLivre->setChecked(true);
        if(livre.value("lu").toInt() == 1)
            ui->checkBoxLuLivre->setChecked(true);
        if(livre.value("ebook").toInt() == 1){
            ui->checkBoxEbookLivre->setChecked(true);
            ui->labelCheminLivre->setText(livre.value("emplacement"));
            ui->pushButtonEmplacementLivre->setEnabled(true);
        }
        ui->horizontalSliderNoteLivre->setValue(livre.value("note").toInt());
        ui->labelNoteLivre->setText(QString::number(livre.value("note").toInt())+"/20");

        if(insSettingsManager->getSettings(Empruntable).toBool())
            ui->checkBoxEmpruntableLivre->setChecked(true);

        //Récupération de l'image
        QPixmap image;
        if(!livre.value("couverture").startsWith("http")){
            QFile fichierImage(livre.value("couverture"));
            if(!fichierImage.exists()){
                QMessageBox::information(this, "Image introuvable", "Une erreur est survenue, la jaquette de ce livre ne peut être trouvée");
            }
            else{
                image.load(livre.value("couverture"));
            }
        }
        else{
            image = ToolsManager::getPixmapFromString(livre.value("couverture"));
            int width = image.width();
            if(width > 150){
                float coef = (float)width / 150;
                int result = image.width()/coef;
                image = image.scaledToWidth(result);
            }
        }
        ui->labelImageLivre->setText(livre.value("couverture"));
        if((livre.value("xml", "-1").toInt() == 1) || insSettingsManager->getSettings(Xml).toBool()){
            ui->comboBoxAuteurLivre->setVisible(false);
            ui->comboBoxEditeurLivre->setVisible(false);
            ui->pushButtonEditAuteurLivre->setVisible(false);
            ui->pushButtonEditEditeurLivre->setVisible(false);
            ui->pushButtonNouvelAuteurLivre->setVisible(false);
            ui->pushButtonNouvelEditeurLivre->setVisible(false);
            if(idEdit)
                ui->listWidgetEtiquettesLivreLivre->addItems(livre.value("etiquettes").split(","));
        }
        else{//Pas XML -> SQL
            this->getAuteur(livre.value("auteur"));
            this->getEditeur(livre.value("editeur"));
            //Màj des étiquettes
            QSqlQuery resultat = insSql->query("SELECT id,nom FROM etiquettes");
            while(resultat.next())
                ui->listWidgetEtiquettesDispoLivre->addItem(resultat.record().value("nom").toString());
            if(idEdit > 0){
                QSqlQuery res1 = insSql->query("SELECT nom FROM etiquettes LEFT JOIN liste_etiquettes ON liste_etiquettes.id_etiquette=etiquettes.id WHERE liste_etiquettes.id_livre="+QString::number(idEdit));
                while(res1.next())
                    ui->listWidgetEtiquettesLivreLivre->addItem(res1.record().value("nom").toString());
            }
        }
        //Étiquettes
        if(insSettingsManager->getSettings(Xml).toBool())
            ui->listWidgetEtiquettesDispoLivre->addItems(insXml->getListEtiquettes());
        else
            ui->listWidgetEtiquettesDispoLivre->addItems(insSql->getListEtiquettes());
    }
    else{ //FILM
        //Nettoyage des QListWidget
       ui->listWidgetActeursFilm->clear();
       ui->listWidgetEtiquettesDispoFilm->clear();
       ui->listWidgetEtiquettesDispoLivre->clear();
       ui->listWidgetEtiquettesFilmFilm->clear();
       ui->listWidgetEtiquettesLivreLivre->clear();
       ui->listWidgetGenreFilm->clear();

        ui->stackedWidget->setCurrentIndex(1);
        ui->lineEditTitreFilm->setText(livre.value("titre"));
        ui->lineEditTitreOriginalFilm->setText(livre.value("titreOriginal"));
        ui->lineEditDirecteurFilm->setText(livre.value("directeur"));
        ui->plainTextEditSynopsisFilm->clear();
        ui->plainTextEditSynopsisFilm->setPlainText(livre.value("synopsis"));
        ui->spinBoxAnneeFilm->setValue(livre.value("annee").toInt());
        ui->spinBoxDureeFilm->setValue(livre.value("duree").toInt());
        ui->lineEditNationaliteFilm->setText(livre.value("pays"));
        ui->lineEditLangueFilm->setText(livre.value("langue"));
        ui->labelImageFilm->setText(livre.value("couverture"));
        ui->lineEditClassementFilm->setText(livre.value("classement"));
        ui->horizontalSliderNoteFilm->setValue((livre.value("note").toInt()));
        ui->labelNoteFilm->setText(livre.value("note"));
        ui->plainTextEditCommentaireFilm->clear();
        ui->plainTextEditCommentaireFilm->setPlainText(livre.value("commentaire"));

        //Récupération de l'image
        QPixmap image;
        if(!livre.value("couverture").startsWith("http")){
            QFile fichierImage(livre.value("couverture"));
            if(!fichierImage.exists()){
                QMessageBox::information(this, "Image introuvable", "Une erreur est survenue, la jaquette de ce livre ne peut être trouvée");
            }
            else{
                image.load(livre.value("couverture"));
            }
        }
        else{
            QNetworkAccessManager nw_manager;
            QNetworkRequest request(livre.value("couverture"));
            QNetworkReply *reponse = nw_manager.get(request);
            QEventLoop eventLoop;
            connect(reponse, SIGNAL(finished()), &eventLoop, SLOT(quit()));
            eventLoop.exec();
            image.loadFromData(reponse->readAll());
            int width = image.width();
            if(width > 150){
                float coef = (float)width / 150;
                int result = image.width()/coef;
                image = image.scaledToWidth(result);
            }
        }

        //Checkbox
        if(livre.value("empruntable", "EE") == "EE"){
            if(insSettingsManager->getSettings(Empruntable).toBool())
                ui->checkBoxEmpruntableFilm->setChecked(true);
        }
        else
            ui->checkBoxEmpruntableFilm->setChecked((livre.value("empruntable") == "True") ? true : false);
        ui->checkBoxPreteFilm->setChecked((livre.value("prete") == "True") ? true : false);
        ui->checkBoxVuFilm->setChecked((livre.value("vu") == "True") ? true : false);
        if(livre.value("fichier") == "True"){
            ui->checkBoxFichierFilm->setChecked(true);
            ui->labelCheminFilm->setText(livre.value("emplacement"));
            ui->pushButtonEmplacementFilm->setEnabled(true);
        }
        //Préparation des variables pour les QListWidget
        if(!livre.value("acteurs").isEmpty()){
            QStringList listActeurs = livre.value("acteurs").split(",");
            for(int i=0; i<listActeurs.count(); ++i){
                ui->listWidgetActeursFilm->addItem(listActeurs.at(i).trimmed());
                ui->listWidgetActeursFilm->setCurrentRow(ui->listWidgetActeursFilm->count()-1);
                ui->listWidgetActeursFilm->currentItem()->setFlags(Qt::ItemIsEditable | Qt::ItemIsEnabled | Qt::ItemIsSelectable);
            }
        }
        if(!livre.value("genre").isEmpty()){
            QStringList listGenres = livre.value("genre").split(",");
            for(int i=0; i<listGenres.count(); ++i){
                ui->listWidgetGenreFilm->addItem(listGenres.at(i).trimmed());
                ui->listWidgetGenreFilm->setCurrentRow(ui->listWidgetGenreFilm->count()-1);
                ui->listWidgetGenreFilm->currentItem()->setFlags(Qt::ItemIsEditable | Qt::ItemIsEnabled | Qt::ItemIsSelectable);
            }
        }
        if(insSettingsManager->getSettings(Xml).toBool())
            ui->listWidgetEtiquettesDispoFilm->addItems(insXml->getListEtiquettes());
        else
            ui->listWidgetEtiquettesDispoFilm->addItems(insSql->getListEtiquettes());

        //Transfert des étiquettes existantes
        if(idEdit > 0){
            QString etiquettes;
            if(livre.value("etiquettes", "FALSE") == "FALSE"){
                //Dans ce cas là, c'est du SQL ou il n'y a pas d'étiquette
                if(!insSettingsManager->getSettings(Xml).toBool()){
                    QSqlQuery res1 = insSql->query("SELECT etiquettes.id,etiquettes.nom FROM etiquettes LEFT JOIN liste_etiquettes ON liste_etiquettes.id_etiquette=etiquettes.id WHERE liste_etiquettes.id_livre="+livre.value("id"));
                    while(res1.next()){
                        etiquettes.append(res1.record().value("nom").toString());
                        etiquettes.append(",");
                    }
                }
            }
            else
                etiquettes = livre.value("etiquettes");
            if(etiquettes.isEmpty())
                return;
            for(int i=0; i<ui->listWidgetEtiquettesDispoFilm->count(); ++i){
                if(etiquettes.contains(ui->listWidgetEtiquettesDispoFilm->item(i)->text())){
                    ui->listWidgetEtiquettesDispoFilm->setCurrentRow(i);
                    this->etiquetteDispoToElem();
                    i--;
                }
            }
        }
    }
    return;
}

//Sélectionne tous les auteurs correspondant au paramètre
void EditBook::getAuteur(QString nom){
    QString req1 = "SELECT id, nom FROM auteurs WHERE nom LIKE ";
    req1 += "'%"+nom.replace(" ", "%") +"%';";

    QSqlQuery res1 = insSql->query(req1);
    while(res1.next()){
        ui->comboBoxAuteurLivre->addItem(res1.record().value("nom").toString(), res1.record().value("id"));
    }
    if(ui->comboBoxAuteurLivre->count() == 0){
        ui->pushButtonEditAuteurLivre->setText("Actualiser");
    }
}

//Sélectionne tous les éditeurs correspondant au paramètre
void EditBook::getEditeur(QString nom){
    QString req1 = "SELECT id, nom FROM editeurs WHERE nom LIKE ";
    req1 += "\"%"+nom.replace(" ", "%") +"%\";";

    QSqlQuery res1 = insSql->query(req1);
        while(res1.next()){
            ui->comboBoxEditeurLivre->addItem(res1.record().value("nom").toString(), res1.record().value("id"));
        }
    if(ui->comboBoxEditeurLivre->count() == 0){
        //ui->pushButton_edit_editeur->setEnabled(false);
        ui->pushButtonEditEditeurLivre->setText("Actualiser");
    }
}

//Actualise la liste des auteurs/éditeurs selon la valeur du champ
void EditBook::setAuteur(bool editeur){
    QString id = "SELECT id,nom FROM ";
    if(editeur){
        id +="editeurs";
    }
    else{
        id +="auteurs";
    }
    id +=" WHERE nom LIKE \"%";
    if(editeur){
        if(ui->pushButtonEditEditeurLivre->text() == "Actualiser")
            id+=ui->lineEditEditeurLivre->text()+"%\";";
        else
            id+=ToolsManager::guillemets(ui->comboBoxEditeurLivre->currentText())+"';";
    }
    else{
        if(ui->pushButtonEditAuteurLivre->text() == "Actualiser")
            id+=ui->lineEditAuteurLivre->text()+"%\"";
        else
            id+=ToolsManager::guillemets(ui->comboBoxAuteurLivre->currentText())+"';";
    }

    QSqlQuery res = insSql->query(id);
    res.next();
    int idl = res.record().value("id").toInt();
    if(idl > 0){
        if(editeur){
            if(ui->pushButtonEditEditeurLivre->text() == "Actualiser"){
                ui->comboBoxEditeurLivre->clear();
                do{
                    ui->comboBoxEditeurLivre->addItem(res.record().value("nom").toString(), res.record().value("id"));
                }while(res.next());
                ui->pushButtonEditEditeurLivre->setText("Éditer l'éditeur");
            }
            else{
                insAddEditeur->setEditeur(idl);
                insAddEditeur->show();
            }
        }
        else{
            if(ui->pushButtonEditAuteurLivre->text() == "Actualiser"){
                ui->comboBoxAuteurLivre->clear();
                do{
                    ui->comboBoxAuteurLivre->addItem(res.record().value("nom").toString(), res.record().value("id"));
                }while(res.next());
                ui->pushButtonEditAuteurLivre->setText("Éditeur l'auteur");
            }
            insAddAuteur->setAuteur(idl);
            insAddAuteur->show();
        }
    }
    else{
        return;
    }
    return;
}

//Appel à setAuteurs avec paramètre pour les éditeurs
void EditBook::setEditeur(){
    this->setAuteur(true);
}

//Met à jour la liste des auteurs
void EditBook::updateAuteurs(int id, QString nom){
    ui->comboBoxAuteurLivre->clear();
    ui->comboBoxAuteurLivre->addItem(nom, id);
    insAddAuteur->close();
}

//Met à jour la liste des éditeurs
void EditBook::updateEditeurs(int id, QString nom){
    ui->comboBoxEditeurLivre->clear();
    ui->comboBoxEditeurLivre->addItem(nom, id);
    insAddEditeur->close();
}

//Récupère l'ID de l'auteur ou de l'éditeur actuels
int EditBook::getId(bool editeur){
    QString id = "SELECT id FROM ";
    if(editeur){
        id +="editeurs";
    }
    else{
        id +="auteurs";
    }
    id +=" WHERE nom LIKE '";
    if(editeur){
        id+=ToolsManager::guillemets(ui->comboBoxEditeurLivre->currentText())+"';";
    }
    else{
        id+=ToolsManager::guillemets(ui->comboBoxAuteurLivre->currentText())+"';";
    }

    QSqlQuery res = insSql->query(id);
    res.next();
    int idl = res.record().value("id").toInt();
    return idl;
}

//Stocke l'ID de l'item édité
void EditBook::setId(int id){
    idEdit = id;
    return;
}

//Lie l'ebook (ou le film) avec l'item
void EditBook::uploadEbook(){
    QString ebook = QFileDialog::getOpenFileName(this, "Sélectionnez le fichier", QDir::homePath());
    ui->labelCheminLivre->setText(ebook);
    return;
}

//Nouvelle étiquette
void EditBook::addEtiquette(){
    if(ui->stackedWidget->currentIndex() == 0){
        ui->listWidgetEtiquettesLivreLivre->addItem("Nouvelle étiquette");
        ui->listWidgetEtiquettesLivreLivre->setCurrentRow(ui->listWidgetEtiquettesLivreLivre->count()-1);
        ui->listWidgetEtiquettesLivreLivre->currentItem()->setFlags(Qt::ItemIsEditable | Qt::ItemIsEnabled | Qt::ItemIsSelectable);
    }
    else{
        ui->listWidgetEtiquettesFilmFilm->addItem("Nouvelle étiquette");
        ui->listWidgetEtiquettesFilmFilm->setCurrentRow(ui->listWidgetEtiquettesFilmFilm->count()-1);
        ui->listWidgetEtiquettesFilmFilm->currentItem()->setFlags(Qt::ItemIsEditable | Qt::ItemIsEnabled | Qt::ItemIsSelectable);
    }
    return;
}

//Transfert d'une étiquette dispo vers l'élément
void EditBook::etiquetteDispoToElem(){
    if(ui->stackedWidget->currentIndex() == 0){
        if(ui->listWidgetEtiquettesDispoLivre->count() == 0 || ui->listWidgetEtiquettesDispoLivre->currentRow() < 0)
            return;
        QString etiquette = ui->listWidgetEtiquettesDispoLivre->currentItem()->text();
        if(!etiquette.isEmpty()){
            ui->listWidgetEtiquettesLivreLivre->addItem(etiquette);
            delete ui->listWidgetEtiquettesDispoLivre->currentItem();
        }
    }
    else{
        if(ui->listWidgetEtiquettesDispoFilm->count() == 0 || ui->listWidgetEtiquettesDispoFilm->currentRow() < 0)
            return;
        QString etiquette = ui->listWidgetEtiquettesDispoFilm->currentItem()->text();
        if(!etiquette.isEmpty()){
            ui->listWidgetEtiquettesFilmFilm->addItem(etiquette);
            delete ui->listWidgetEtiquettesDispoFilm->currentItem();
        }
    }
    return;
}

//Transfert d'une étiquette de l'élément vers les disponibles
void EditBook::etiquetteElemToDispo(){
    if(ui->stackedWidget->currentIndex() == 0){
        if(ui->listWidgetEtiquettesLivreLivre->count() == 0 || ui->listWidgetEtiquettesLivreLivre->currentRow() < 0)
            return;
        QString etiquette = ui->listWidgetEtiquettesLivreLivre->currentItem()->text();
        if(!etiquette.isEmpty()){
            ui->listWidgetEtiquettesDispoLivre->addItem(etiquette);
            delete ui->listWidgetEtiquettesLivreLivre->currentItem();
        }
    }
    else{
        if(ui->listWidgetEtiquettesFilmFilm->count() == 0 || ui->listWidgetEtiquettesFilmFilm->currentRow() < 0)
            return;
        QString etiquette = ui->listWidgetEtiquettesFilmFilm->currentItem()->text();
        if(!etiquette.isEmpty()){
            ui->listWidgetEtiquettesDispoFilm->addItem(etiquette);
            delete ui->listWidgetEtiquettesFilmFilm->currentItem();
        }
    }
    return;
}

//Ajoute un nouvel acteur
void EditBook::addActeur(){
    ui->listWidgetActeursFilm->addItem("Nouvel acteur");
    ui->listWidgetActeursFilm->setCurrentRow(ui->listWidgetActeursFilm->count()-1);
    ui->listWidgetActeursFilm->currentItem()->setFlags(Qt::ItemIsEditable | Qt::ItemIsEnabled | Qt::ItemIsSelectable);
    return;
}

//Ajoute un nouveau genre
void EditBook::addGenre(){
    ui->listWidgetGenreFilm->addItem("Nouveau genre");
    ui->listWidgetGenreFilm->setCurrentRow(ui->listWidgetGenreFilm->count()-1);
    ui->listWidgetGenreFilm->currentItem()->setFlags(Qt::ItemIsEditable | Qt::ItemIsEnabled | Qt::ItemIsSelectable);
    return;
}

//Vide tous les champs
void EditBook::clearAll(){
    bool films = ((QString::compare(insSettingsManager->getSettings(Type).toString(), "films", Qt::CaseInsensitive) != 0) ? false : true);
    if(films){
        ui->lineEditTitreFilm->clear();
        ui->lineEditTitreOriginalFilm->clear();
        ui->lineEditDirecteurFilm->clear();
        ui->lineEditNationaliteFilm->clear();
        ui->spinBoxAnneeFilm->clear();
        ui->spinBoxDureeFilm->clear();
        ui->listWidgetActeursFilm->clear();
        ui->listWidgetGenreFilm->clear();
        ui->labelCheminFilm->clear();
        ui->labelImageFilm->clear();
        ui->listWidgetEtiquettesFilmFilm->clear();
        ui->listWidgetEtiquettesDispoFilm->clear();
        ui->plainTextEditCommentaireFilm->clear();
        ui->plainTextEditSynopsisFilm->clear();
    }
    else{ //Livres
        ui->lineEditTitreLivre->clear();
        ui->lineEditAuteurLivre->clear();
        ui->lineEditEditeurLivre->clear();
        ui->comboBoxAuteurLivre->clear();
        ui->comboBoxEditeurLivre->clear();
        ui->spinBoxAnneeLivre->clear();
        ui->spinBoxPagesLivre->clear();
        ui->lineEditCoAuteurLivre->clear();
        ui->lineEditISBNLivre->clear();
        ui->lineEditLangueLivre->clear();
        ui->labelCheminLivre->clear();
        ui->labelImageLivre->clear();
        ui->listWidgetEtiquettesLivreLivre->clear();
        ui->listWidgetEtiquettesDispoLivre->clear();
        ui->plainTextEditCommentaireLivre->clear();
        ui->plainTextEditSynopsisLivre->clear();
    }
    return;
}
