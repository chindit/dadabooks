#include "lenddialog.h"
#include "ui_lenddialog.h"

LendDialog::LendDialog(QWidget *parent) : QDialog(parent), ui(new Ui::LendDialog){
    ui->setupUi(this);
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
        emit lendCurrent();
    }
    this->close();
}
