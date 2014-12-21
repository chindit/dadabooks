#include "lenddialog.h"
#include "ui_lenddialog.h"

LendDialog::LendDialog(QWidget *parent) : QDialog(parent), ui(new Ui::LendDialog){
    ui->setupUi(this);
    ui->pushButtonRappel->setHidden(true);

    connect(ui->pushButtonRendu, SIGNAL(clicked()), this, SLOT(sendReturned()));
}

LendDialog::~LendDialog(){
    delete ui;
}

void LendDialog::setAction(lendAction lact){
    if(lact == Return)
        ui->stackedWidget->setCurrentIndex(0);
    else
        ui->stackedWidget->setCurrentIndex(1);
    return;
}

void LendDialog::setTitle(QString title){
    ui->labelContenuPret->setText(title);
    ui->labelTitre->setText(title);
    this->titre = title;
    return;
}

void LendDialog::accept(){
    if(ui->stackedWidget->currentIndex() == 0){
        //RETURN
    }
    else{
        //LEND
        if(ui->lineEditNom->text().isEmpty()){
            QMessageBox::warning(this, tr("Nom vide"), tr("Le nom de l'emprunteur ne peut pas être vide"));
            return;
        }
        if(ui->lineEditEmail->text().isEmpty()){
            QMessageBox::warning(this, tr("Email vide"), tr("L'adresse email ne peut pas être vide"));
            return;
        }
        QRegExp courriel("\\b[A-Z0-9._%+-]+@[A-Z0-9.-]+\\.[A-Z]{2,4}\\b");
        courriel.setCaseSensitivity(Qt::CaseInsensitive);
        courriel.setPatternSyntax(QRegExp::RegExp);
        if(!courriel.exactMatch(ui->lineEditEmail->text())){
            QMessageBox::warning(this, tr("Email invalide"), tr("L'adresse email fournie est invalide"));
            return;
        }
        emit lendCurrent(ui->lineEditNom->text().trimmed(), ui->lineEditEmail->text().trimmed());
    }
    this->close();
    return;
}

void LendDialog::setBorrower(int id, int idItem, QString nom, QString email, QDate dateEmprunt){
    this->idPret = id;
    this->idItem = idItem;
    this->courriel = email;
    ui->labelContenu->setText(tr("%1 a emprunté %2 le %3.").arg(nom).arg(this->titre).arg(dateEmprunt.toString("dd/MM/yy")));
    return;
}

void LendDialog::sendReturned(){
    emit returnCurrent(this->idPret, this->idItem);
    this->close();
    return;
}
