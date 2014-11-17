#include "addediteur.h"
#include "ui_addediteur.h"

AddEditeur::AddEditeur(QWidget *parent) : QDialog(parent), ui(new Ui::AddEditeur){
    ui->setupUi(this);
    isEdit = 0;
    insSql = new SqlManager;
}

AddEditeur::~AddEditeur(){
    delete ui;
    //delete insSql;
}

void AddEditeur::accept(){
    QString nom = ui->lineEdit_nom->text();
    QString adresse = ui->textEdit_adresse->toPlainText();
    QString site = ui->lineEdit_site->text();

    QSqlQuery res1;
    QString req1 = "INSERT INTO editeurs(nom, adresse, site) VALUES('"+ToolsManager::guillemets(nom)+"', \""+ToolsManager::guillemets(adresse)+"\", '"+ToolsManager::guillemets(site)+"');";
    if(isEdit > 0)
        req1 = "UPDATE editeurs SET nom = '"+ToolsManager::guillemets(nom)+"', adresse ='"+ToolsManager::guillemets(adresse)+"', site = '"+ToolsManager::guillemets(site)+"' WHERE id="+QString::number(isEdit)+";";
    res1 = insSql->query(req1);
    if(isEdit == 0)
        isEdit = res1.lastInsertId().toInt();
    emit makeClose(isEdit, nom);
}

void AddEditeur::setEditeur(int id){
    ui->label->setText("Éditer un éditeur");
    QString req1 = "SELECT * FROM editeurs WHERE id="+QString::number(id)+";";

    QSqlQuery res1 = insSql->query(req1);
    res1.next();
    ui->lineEdit_nom->setText(res1.record().value("nom").toString());
    ui->lineEdit_site->setText(res1.record().value("site").toString());
    ui->textEdit_adresse->setText(res1.record().value("adresse").toString());
    isEdit = res1.record().value("id").toInt();
    return;
}

int AddEditeur::setEditeur(QString nom){
    QString req1 = "INSERT INTO editeurs(nom, adresse, site) VALUES(\""+ToolsManager::guillemets(nom)+"\", \"\", \"\");";
    QSqlQuery res1 = insSql->query(req1);
    return res1.lastInsertId().toInt();
}
