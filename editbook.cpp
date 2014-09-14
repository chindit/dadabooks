#include "editbook.h"
#include "ui_editbook.h"

EditBook::EditBook(QWidget *parent) : QDialog(parent), ui(new Ui::EditBook){
    ui->setupUi(this);
    idEdit = 0;
    insSettingsManager = new SettingsManager(this);
    if(insSettingsManager->getSettings(Xml).toBool()){
        insXml = new XmlManager;
    }
    else{
        insSql = new SqlManager();
        insAddAuteur = new AddAuteur(this);
        insAddEditeur = new AddEditeur(this);
        connect(ui->pushButton_auteur, SIGNAL(clicked()), insAddAuteur, SLOT(show()));
        connect(insAddAuteur, SIGNAL(makeClose(int, QString)), this, SLOT(updateAuteurs(int, QString)));
        connect(ui->pushButton_editeur, SIGNAL(clicked()), insAddEditeur, SLOT(show()));
        connect(insAddEditeur, SIGNAL(makeClose(int, QString)), this, SLOT(updateEditeurs(int, QString)));
    }

    //Gestion des films
    if(insSettingsManager->getSettings(Type).toString() == "films"){
        ui->stackedWidget->setCurrentIndex(1);
        ui->labelTitreGeneral->setText("Édition du film");
    }
    else{
        connect(ui->pushButton_emplacement, SIGNAL(clicked()), this, SLOT(uploadEbook()));
    }
}

EditBook::~EditBook(){
    if(insSettingsManager->getSettings(Xml).toBool()){
        delete insXml;
    }
    else{
        delete insSql;
        delete insAddAuteur;
        delete insAddEditeur;
    }
    delete ui;
    delete insSettingsManager;
}

void EditBook::accept(){
    if(QString::compare(insSettingsManager->getSettings(Type).toString(), "films", Qt::CaseInsensitive) != 0){
        int id_auteur = 0, id_editeur = 0;
        //1)Vérification de la présence de l'auteur
        if(ui->comboBox_auteur->currentText().isEmpty() && !insSettingsManager->getSettings(Xml).toBool()){
            if(insSettingsManager->getSettings(AutoAuteur).toBool()){
                id_auteur = insAddAuteur->setAuteur(ui->lineEdit_auteur->text());
            }
            else{
                QMessageBox::warning(this, "Auteur manquant", "L'auteur de ce livre n'est pas renseigné.  Veuillez créer une nouvelle entrée");
                return;
            }
        }
        //2)Vérification de la présence de l'éditeur
        if(ui->comboBox_editeur->currentText().isEmpty() && !insSettingsManager->getSettings(Xml).toBool()){
            if(insSettingsManager->getSettings(AutoEditeur).toBool()){
                id_editeur = insAddEditeur->setEditeur(ui->lineEdit_editeur->text());
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
            id_editeur=this->getId(true);
            if(id_auteur <= 0 || id_auteur <= 0){
                QMessageBox::warning(this, "Erreur étrange", "L'auteur ou l'éditeur n'ont pu être trouvés dans la base de données.  Essayez en ré-insérant le nom ou en redémarrant le programme");
                return;
            }
        }
        //4)Vérification du titre
        if(ui->lineEdit_titre->text().isEmpty()){
            QMessageBox::critical(this, "Impossible d'ajouter le livre", "Nous sommes désolé mais il est impossible d'ajouter un livre qui ne possède pas de titre");
            return;
        }
        int annee, pages, edition, exemplaires, empruntable, pret, lu, note, ebook;
        annee = ui->spinBox_annee->value();
        pages = ui->spinBox_pages->value();
        edition = ui->spinBox_edition->value();
        exemplaires = ui->spinBox_exemplaires->value();
        empruntable = (ui->checkBox_empruntable->isChecked()) ? 1 : 0 ;
        pret = (ui->checkBox_prete->isChecked()) ? 1 : 0 ;
        lu = (ui->checkBox_lu->isChecked()) ? 1 : 0;
        ebook = (ui->checkBox_ebook->isChecked() ? 1 : 0);
        note = ui->horizontalSlider_note->value();
        QString req1;
        int idLivre = -1;

        QString nomImage = ui->label_image_texte->text();
        //Stockage de l'image en local en cas de besoin
        if(!insSettingsManager->getSettings(MariaDB).toBool() && insSettingsManager->getSettings(DownPics).toBool()){
            //On vérifie que le dossier existe
            QString chemin = insSettingsManager->getSettings(Fichier).toString();
            chemin = chemin.left(chemin.lastIndexOf("/")+1);
            QDir dossierImage; dossierImage.setPath(chemin+".dadabooksImages");
            if(!dossierImage.exists()){
                dossierImage.mkdir(chemin+".dadabooksImages");
            }

            //Et on télécharge les images
            QNetworkAccessManager nw_manager;
            QNetworkRequest request(ui->label_image_texte->text());
            QNetworkReply *reponse = nw_manager.get(request);
            QEventLoop eventLoop;
            connect(reponse, SIGNAL(finished()), &eventLoop, SLOT(quit()));
            eventLoop.exec();
            QPixmap image;
            image.loadFromData(reponse->readAll());
            int width = image.width();
            if(width > 150){
                float coef = (float)width / 150;
                int result = image.width()/coef;
                image = image.scaledToWidth(result);
            }
            QDateTime timestamp; timestamp = QDateTime::currentDateTime();
            nomImage = chemin+".dadabooksImages/"+QString::number(timestamp.toTime_t())+".png";
            image.save(nomImage);
        }

        if(!insSettingsManager->getSettings(Xml).toBool()){
            if(idEdit == 0){
                req1 = "INSERT INTO livres(titre, ISBN, auteur, coauteurs, synopsis, couverture, editeur, annee, pages, edition, langue, classement, exemplaires, commentaire, lu, note, empruntable, pret, ebook, emplacement) VALUES(\""+ui->lineEdit_titre->text().replace("\"", "\\\"")+"\", \""+ui->lineEdit_ISBN->text()+"\", "+QString::number(id_auteur)+", \""+this->guillemets(ui->lineEdit_coauteurs->text())+"\", \""+this->guillemets(ui->plainTextEdit_resume->toPlainText())+"\", \""+ui->label_image_texte->text()+"\", "+QString::number(id_editeur)+", '"+QString::number(annee)+"', "+QString::number(pages)+", "+QString::number(edition)+", \""+this->guillemets(ui->lineEdit_langue->text())+"\", \""+this->guillemets(ui->lineEdit_classement->text())+"\", "+QString::number(exemplaires)+", \""+this->guillemets(ui->plainTextEdit_commentaire->toPlainText())+"\", "+QString::number(lu)+", "+QString::number(note)+", "+QString::number(empruntable)+", "+QString::number(pret)+", "+QString::number(ebook)+", \""+ui->lineEdit_emplacement->text()+"\");";
            }
            else{
                req1 = "UPDATE livres SET titre = \""+this->guillemets(ui->lineEdit_titre->text())+"\", ISBN = \""+ui->lineEdit_ISBN->text()+"\", auteur = "+QString::number(id_auteur)+", coauteurs = \""+this->guillemets(ui->lineEdit_coauteurs->text())+"\", synopsis = \""+this->guillemets(ui->plainTextEdit_resume->toPlainText())+"\", couverture = \""+nomImage+"\", editeur = "+QString::number(id_editeur)+", annee = '"+QString::number(annee)+"', pages = "+QString::number(pages)+", edition = "+QString::number(edition)+", langue = \""+this->guillemets(ui->lineEdit_langue->text())+"\", classement = \""+this->guillemets(ui->lineEdit_classement->text())+"\", exemplaires = "+QString::number(exemplaires)+", commentaire = \""+this->guillemets(ui->plainTextEdit_commentaire->toPlainText())+"\", lu = "+QString::number(lu)+", note = "+QString::number(note)+", empruntable = "+QString::number(empruntable)+", pret = "+QString::number(pret)+", ebook = "+QString::number(ebook)+", emplacement = \""+ui->lineEdit_emplacement->text()+"\" WHERE id="+QString::number(idEdit)+";";
            }

            QSqlQuery res1 = insSql->query(req1);
            idLivre = (idEdit == 0) ? res1.lastInsertId().toInt() : idEdit;
            QString etiquette = ui->comboBox_etiquette->currentText();
            QStringList etiquettes;
            if(etiquette.contains(",")){
                etiquettes = etiquette.split(",");
            }
            else
                etiquettes.append(etiquette);
            if(!etiquette.isEmpty()){
                int id = -1;
                //On vire les vieilles étiquettes
                insSql->query("DELETE FROM liste_etiquettes WHERE id_livre="+QString::number(idLivre));
                for(int i=0; i<etiquettes.size(); i++){
                    req1 = "SELECT id FROM etiquettes WHERE nom=\""+etiquettes.at(i).trimmed()+"\"";
                    res1 = insSql->query(req1);
                    if(res1.size() < 1){
                        req1 = "INSERT INTO etiquettes(nom) VALUES(\""+etiquettes.at(i)+"\")";
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
            //XML
            QMultiMap<QString, QString> livre;
            livre.insert("titre", ui->lineEdit_titre->text());
            livre.insert("isbn", ui->lineEdit_ISBN->text());
            livre.insert("auteur", ui->lineEdit_auteur->text());
            livre.insert("editeur", ui->lineEdit_editeur->text());
            livre.insert("coauteur", ui->lineEdit_coauteurs->text());
            livre.insert("couverture", nomImage);
            livre.insert("pages", QString::number(ui->spinBox_pages->value()));
            livre.insert("edition", QString::number(ui->spinBox_edition->value()));
            livre.insert("langue", ui->lineEdit_langue->text());
            livre.insert("exemplaires", QString::number(ui->spinBox_exemplaires->value()));
            livre.insert("classement", ui->lineEdit_classement->text());
            livre.insert("annee", QString::number(ui->spinBox_annee->value()));
            livre.insert("synopsis", ui->plainTextEdit_resume->toPlainText());
            livre.insert("commentaire", ui->plainTextEdit_commentaire->toPlainText());
            livre.insert("note", QString::number(ui->horizontalSlider_note->value()));
            livre.insert("empruntable", ((ui->checkBox_empruntable->isChecked()) ? "True" : "False"));
            livre.insert("prete", ((ui->checkBox_prete->isChecked()) ? "True" : "False"));
            livre.insert("lu", ((ui->checkBox_lu->isChecked()) ? "True" : "False"));
            livre.insert("ebook", ((ui->checkBox_ebook->isChecked()) ? "True" : "False"));
            if(ui->checkBox_ebook->isChecked())
                livre.insert("emplacement", ui->lineEdit_emplacement->text());

            if(idEdit == 0){
                insXml->addBook(livre);
            }
            else{
                livre.insert("id", QString::number(idEdit));
                insXml->deleteBook(idEdit);
                insXml->addBook(livre);
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
    else{
        //C'es un FILM

        //1)Vérification du titre
        if(ui->lineEditTitre->text().isEmpty()){
            QMessageBox::critical(this, "Pas de titre", "Nous sommes désolé mais il est impossible d'ajouter un film qui ne possède pas de titre");
            return;
        }

        //2)On stocke tout en SQL ou XML
        if(insSettingsManager->getSettings(Xml).toBool()){
            //XML
            QMultiMap<QString, QString> film;
            film.insert("titre", ui->lineEditTitre->text());
            film.insert("titreOriginal", ui->lineEditOriginal->text());
            film.insert("directeur", ui->lineEditDirecteur->text());
            film.insert("acteurs", ui->lineEditActeurs->text());
            film.insert("synopsis", ui->editResume->toPlainText());
            film.insert("annee", QString::number(ui->spinBoxAnnee->value()));
            film.insert("duree", QString::number(ui->spinBoxDuree->value()));
            film.insert("type", ui->lineEditType->text());
            film.insert("pays", ui->lineEditNationalite->text());
            film.insert("langue", ui->lineEditLangue->text());
            film.insert("classement", ui->lineEditClassement->text());
            film.insert("commentaire", ui->editCommentaire->toPlainText());
            film.insert("note", QString::number(ui->horizontalSlider->value()));
            film.insert("couverture", ui->labelTxtImage->text());
            film.insert("empruntable", ((ui->checkBoxEmpruntable->isChecked()) ? "True" : "False"));
            film.insert("prete", ((ui->checkBoxPrete->isChecked()) ? "True" : "False"));
            film.insert("vu", ((ui->checkBoxVu->isChecked()) ? "True" : "False"));
            film.insert("fichier", ((ui->checkBoxFichier->isChecked()) ? "True" : "False"));
            if(ui->checkBoxFichier->isChecked())
                film.insert("emplacement", ui->lineEditEmplacement->text());

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
        else{
            //SQL
        }

    }
    this->close();
    return;
}

void EditBook::setManual(){
    if(ui->stackedWidget->currentIndex() == 1){
        ui->labelImage->setText("<a href=\"#\">Cliquez ici pour ajouter une jaquette</a>");
        connect(ui->labelImage, SIGNAL(linkActivated(QString)), this, SLOT(uploadImage()));
    }
    else{
        ui->label_image->setText("<a href=\"#\">Cliquez ici pour ajouter une image</a>");
        connect(ui->label_image, SIGNAL(linkActivated(QString)), this, SLOT(uploadImage()));
    }
}

void EditBook::uploadImage(){
    QString image = QFileDialog::getOpenFileName(this, "Choisir une photo", QString(), "Images (*.png *.gif *.jpg *.jpeg)");
    if(ui->stackedWidget->currentIndex() == 1)
        ui->labelImage->setText(image);
    else
        ui->label_image_texte->setText(image);
    return;
}

void EditBook::updateUi(QMultiMap<QString, QString> livre){
    ui->lineEdit_titre->setText(livre.value("titre"));
    ui->lineEdit_auteur->setText(livre.value("auteur"));
    ui->lineEdit_editeur->setText(livre.value("editeur"));
    ui->spinBox_annee->setValue(livre.value("annee").toInt());
    ui->lineEdit_ISBN->setText(livre.value("isbn"));
    ui->lineEdit_langue->setText(livre.value("langue"));
    ui->spinBox_pages->setValue(livre.value("pages").toInt());
    ui->plainTextEdit_resume->clear();
    ui->plainTextEdit_resume->insertPlainText(livre.value("synopsis"));
    ui->lineEdit_coauteurs->setText(livre.value("coauteurs", ""));
    ui->spinBox_edition->setValue(livre.value("edition", "1").toInt());
    ui->spinBox_exemplaires->setValue(livre.value("exemplaires", "1").toInt());
    ui->plainTextEdit_commentaire->clear();
    ui->plainTextEdit_commentaire->insertPlainText(livre.value("commentaire", ""));
    ui->lineEdit_classement->setText(livre.value("classement", ""));
    if(livre.value("empruntable", "0").toInt() == 1)
        ui->checkBox_empruntable->setChecked(true);
    if(livre.value("prete", "0").toInt() == 1)
        ui->checkBox_prete->setChecked(true);
    if(livre.value("lu").toInt() == 1)
        ui->checkBox_lu->setChecked(true);
    if(livre.value("ebook").toInt() == 1){
        ui->checkBox_ebook->setChecked(true);
        ui->lineEdit_emplacement->setText(livre.value("emplacement"));
        ui->pushButton_emplacement->setEnabled(true);
    }
    ui->horizontalSlider_note->setValue(livre.value("note").toInt());
    ui->label_note->setText(QString::number(livre.value("note").toInt())+"/20");

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
    ui->label_image->setPixmap(image);
    ui->label_image_texte->setText(livre.value("couverture"));
    ui->label_image_texte->setMaximumWidth(150);
    ui->label_image_texte->setVisible(true);
    if((livre.value("xml", "-1").toInt() == 1) || insSettingsManager->getSettings(Xml).toBool()){
        ui->comboBox_auteur->setVisible(false);
        ui->comboBox_editeur->setVisible(false);
        ui->pushButton_edit_auteur->setVisible(false);
        ui->pushButton_edit_editeur->setVisible(false);
        ui->pushButton_auteur->setVisible(false);
        ui->pushButton_editeur->setVisible(false);
        ui->label_etiquette->setVisible(false);
        ui->comboBox_etiquette->setVisible(false);
    }
    else{//Pas XML -> SQL
        this->getAuteur(livre.value("auteur"));
        this->getEditeur(livre.value("editeur"));
        //Màj des étiquettes
        QSqlQuery resultat = insSql->query("SELECT id,nom FROM etiquettes");
        while(resultat.next()){
            ui->comboBox_etiquette->addItem(resultat.record().value("nom").toString(), resultat.record().value("id"));
        }
    }
    return;
}

void EditBook::getAuteur(QString nom){
    QString req1 = "SELECT id, nom FROM auteurs WHERE nom LIKE ";
    req1 += "'%"+nom.replace(" ", "%") +"%';";

    QSqlQuery res1 = insSql->query(req1);
    while(res1.next()){
        ui->comboBox_auteur->addItem(res1.record().value("nom").toString(), res1.record().value("id"));
    }
    if(ui->comboBox_auteur->count() == 0){
        //ui->pushButton_edit_auteur->setEnabled(false);
        ui->pushButton_edit_auteur->setText("Actualiser");
    }
}

void EditBook::getEditeur(QString nom){
    QString req1 = "SELECT id, nom FROM editeurs WHERE nom LIKE ";
    req1 += "\"%"+nom.replace(" ", "%") +"%\";";

    QSqlQuery res1 = insSql->query(req1);
        while(res1.next()){
            ui->comboBox_editeur->addItem(res1.record().value("nom").toString(), res1.record().value("id"));
        }
    if(ui->comboBox_editeur->count() == 0){
        //ui->pushButton_edit_editeur->setEnabled(false);
        ui->pushButton_edit_editeur->setText("Actualiser");
    }
}

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
        if(ui->pushButton_edit_editeur->text() == "Actualiser")
            id+=ui->lineEdit_editeur->text()+"%\";";
        else
            id+=this->guillemets(ui->comboBox_editeur->currentText())+"';";
    }
    else{
        if(ui->pushButton_edit_auteur->text() == "Actualiser")
            id+=ui->lineEdit_auteur->text()+"%\"";
        else
            id+=this->guillemets(ui->comboBox_auteur->currentText())+"';";
    }

    QSqlQuery res = insSql->query(id);
    res.next();
    int idl = res.record().value("id").toInt();
    if(idl > 0){
        if(editeur){
            if(ui->pushButton_edit_editeur->text() == "Actualiser"){
                ui->comboBox_editeur->clear();
                do{
                    ui->comboBox_editeur->addItem(res.record().value("nom").toString(), res.record().value("id"));
                }while(res.next());
                ui->pushButton_edit_editeur->setText("Éditer l'éditeur");
            }
            else{
                insAddEditeur->setEditeur(idl);
                insAddEditeur->show();
            }
        }
        else{
            if(ui->pushButton_edit_auteur->text() == "Actualiser"){
                ui->comboBox_auteur->clear();
                do{
                    ui->comboBox_auteur->addItem(res.record().value("nom").toString(), res.record().value("id"));
                }while(res.next());
                ui->pushButton_edit_auteur->setText("Éditeur l'auteur");
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

void EditBook::setEditeur(){
    this->setAuteur(true);
}

void EditBook::updateAuteurs(int id, QString nom){
    ui->comboBox_auteur->clear();
    ui->comboBox_auteur->addItem(nom, id);
    insAddAuteur->close();
}

void EditBook::updateEditeurs(int id, QString nom){
    ui->comboBox_editeur->clear();
    ui->comboBox_editeur->addItem(nom, id);
    insAddEditeur->close();
}

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
        id+=this->guillemets(ui->comboBox_editeur->currentText())+"';";
    }
    else{
        id+=this->guillemets(ui->comboBox_auteur->currentText())+"';";
    }

    QSqlQuery res = insSql->query(id);
    res.next();
    int idl = res.record().value("id").toInt();
    return idl;
}

void EditBook::setId(int id){
    idEdit = id;
    return;
}

QString EditBook::guillemets(QString input){
    QString output = input;
    bool close = false;
    for(int i=0; i<input.size(); i++){
        if(output.at(i) == QChar('"')){
            if(close){
                output.replace(i, 1, "»");
                close = false;
            }
            else{
                output.replace(i, 1, "«");
                close = true;
            }
        }
    }
    return output;
}

void EditBook::updateNote(int value){
    ui->label_note->setText(QString::number(value)+"/20");
    return;
}

void EditBook::uploadEbook(){
    QString ebook = QFileDialog::getOpenFileName(this, "Sélectionnez le fichier", QDir::homePath());
    ui->lineEdit_emplacement->setText(ebook);
    return;
}
