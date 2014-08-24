#include "addauteur.h"
#include "ui_addauteur.h"

AddAuteur::AddAuteur(SqlManager *instance, QWidget *parent) : QDialog(parent), ui(new Ui::AddAuteur){
    ui->setupUi(this);
    isEdit = 0;
    insSql = instance;
}

AddAuteur::~AddAuteur(){
    delete ui;
}

void AddAuteur::accept(){
    QString nom = ui->lineEdit_nom->text();
    long naissance = ui->spinBox->value();
    long mort = ui->spinBox_2->value();
    QString biographie = ui->textEdit->toPlainText();
    biographie = biographie.replace("\"", "\\");
    QString photo = ui->label_fichier->text();

    QSqlQuery res1;
    QString req1 = "INSERT INTO auteurs(nom, naissance, mort, biographie, photo) VALUES('"+nom.replace("'", "\\'")+"', '"+QString::number(naissance)+"', '"+QString::number(mort)+"', \""+biographie.replace("\"", "\\\"")+"\", '"+photo+"');";
    if(isEdit > 0){
        req1 = "UPDATE auteurs SET nom = '"+nom.replace("'", "\\'")+"', naissance ='"+QString::number(naissance)+"', mort = '"+QString::number(mort)+"', biographie = \""+biographie.replace("'", "\\'")+"\", photo='"+photo+"' WHERE id="+QString::number(isEdit)+";";
        isEdit = 0;
    }
    res1 = insSql->query(req1);
    emit makeClose();
}

void AddAuteur::setImage(){
    QString fichier = QFileDialog::getOpenFileName(this, "Choisir une photo", QString(), "Images (*.png *.gif *.jpg *.jpeg)");
    ui->label_fichier->setText(fichier);
}

void AddAuteur::setAuteur(int id){
    ui->label->setText("Éditer un auteur");
    QString req1 = "SELECT * FROM auteurs WHERE id="+QString::number(id)+";";

    QSqlQuery res1 = insSql->query(req1);
    res1.next();
    ui->lineEdit_nom->setText(res1.record().value("nom").toString());
    ui->textEdit->insertPlainText(res1.record().value("biographie").toString());
    ui->spinBox->setValue(res1.record().value("naissance").toInt());
    ui->spinBox_2->setValue(res1.record().value("mort").toInt());
    ui->label_fichier->setText(res1.record().value("image").toString());
    isEdit = res1.record().value("id").toInt();
    return;
}

int AddAuteur::setAuteur(QString nom){
    //Ajoute automatiquement un auteur et retourne son ID
    QString req1 = "INSERT INTO auteurs(nom, naissance, mort, biographie, photo) VALUES('"+nom.replace("'", "\\'")+"', '', '', \"\", '');";
    QSqlQuery res1 = insSql->query(req1);
    return res1.lastInsertId().toInt();
}
