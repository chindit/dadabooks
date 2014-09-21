#include "previewbook.h"
#include "ui_previewbook.h"

PreviewBook::PreviewBook(QWidget *parent) : QDialog(parent), ui(new Ui::PreviewBook){
    insSettingsManager = new SettingsManager();
    ui->setupUi(this);
}

PreviewBook::~PreviewBook(){
    delete ui;
    delete insSettingsManager;
}

void PreviewBook::setTable(QList<QMultiMap<QString, QString> > elem){
    QDialog *waiting = new QDialog(this);
    waiting->setWindowIcon(QIcon(":/programme/images/icone.png"));
    waiting->setWindowModality(Qt::ApplicationModal);
    QVBoxLayout *layout = new QVBoxLayout;
    waiting->setLayout(layout);
    QProgressBar *bar = new QProgressBar;
    QLabel *texte = new QLabel("Téléchargement des aperçus");
    layout->addWidget(texte);
    layout->addWidget(bar);
    bar->setValue(0);
    waiting->show();
    tableau = elem;
    ui->label_nombre->setText(QString::number(elem.size()));
    ui->tableWidget->clear();
    ui->tableWidget->setRowCount(elem.size()-1);
    ui->tableWidget->setColumnCount(6);
    QStringList liste_headers;
    liste_headers << "Image" << "Titre" << ((QString::compare(insSettingsManager->getSettings(Type).toString(), "films", Qt::CaseInsensitive) != 0) ? "Auteur" : "Directeur") << ((QString::compare(insSettingsManager->getSettings(Type).toString(), "films", Qt::CaseInsensitive) != 0) ? "Éditeur" : "Acteurs") << "Année" << "Enregistrer";
    ui->tableWidget->setHorizontalHeaderLabels(liste_headers);
    for (int row = 0; row < elem.size(); ++row) {
        bar->setValue((row*100)/elem.size());
        bar->show();
        QNetworkAccessManager nw_manager;
        QNetworkRequest request(elem.at(row).value("image"));
        QNetworkReply *reponse = nw_manager.get(request);
        QEventLoop eventLoop;
        connect(reponse, SIGNAL(finished()), &eventLoop, SLOT(quit()));
        eventLoop.exec();
        QPixmap image, image2;
        image.loadFromData(reponse->readAll());
        image2.load(":/boutons/images/ok.png");
        QTableWidgetItem *item1 = new QTableWidgetItem();
        item1->setData(Qt::DecorationRole, image);
        QTableWidgetItem *item2 = new QTableWidgetItem(elem.at(row).value("titre"));
        QTableWidgetItem *item3 = new QTableWidgetItem(elem.at(row).value("auteur"));
        QTableWidgetItem *item4 = new QTableWidgetItem(elem.at(row).value("editeur"));
        QTableWidgetItem *item5 = new QTableWidgetItem(elem.at(row).value("annee"));
        QTableWidgetItem *item6 = new QTableWidgetItem();
        item6->setData(Qt::DecorationRole, image2);
        //WARNING : J'ignore pourquoi cette condition est nécessaire mais sans elle,
        // le dernier résultat ne s'affiche pas.  Encore un mystère de l'informatique.
        if(row == (elem.size()-1))
            ui->tableWidget->insertRow(row);
        ui->tableWidget->setItem(row, 0, item1);
        ui->tableWidget->setItem(row, 1, item2);
        ui->tableWidget->setItem(row, 2, item3);
        ui->tableWidget->setItem(row, 3, item4);
        ui->tableWidget->setItem(row, 4, item5);
        ui->tableWidget->setItem(row, 5, item6);
        ui->tableWidget->setRowHeight(row, 150);
    }
    waiting->close();
    delete waiting;

    //On stocke tout dans la QTableView
    ui->tableWidget->show();

    //On adapte la fenêtre aux résultats
    ui->tableWidget->setAutoScroll(false);
    ui->tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);

    //On connecte le tableau
    connect(ui->tableWidget, SIGNAL(cellClicked(int,int)), this, SLOT(detectClic(int,int)));
    return;
}

void PreviewBook::detectClic(int row, int column){
    if(column == 5)
        emit bookSelected(tableau.at(row).value("id"));
    return;
}
