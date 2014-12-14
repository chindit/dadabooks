#include "imageuploaddialog.h"
#include "ui_imageuploaddialog.h"

ImageUploadDialog::ImageUploadDialog(QWidget *parent) : QDialog(parent), ui(new Ui::ImageUploadDialog){
    ui->setupUi(this);
    connect(ui->pushButtonChoose, SIGNAL(clicked()), this, SLOT(selectImage()));
    connect(ui->pushButtonTest, SIGNAL(clicked()), this, SLOT(testImage()));
}

ImageUploadDialog::~ImageUploadDialog(){
    delete ui;
}

QString ImageUploadDialog::getImage(){
    if(image.isEmpty() && ui->lineEdit->text().isEmpty())
        return QString();
    else{
        if(!image.isEmpty()){
            return ((this->testFichier()) ? image : QString());
        }
        else{
            if(ui->lineEdit->text().startsWith("http"))
                return ui->lineEdit->text();
            else
                return QString();
        }
    }
    return QString();
}

void ImageUploadDialog::testImage(){
    if(!image.isEmpty()){
        if(!this->testFichier()){
            return;
        }
    }
    else{
        if(!ui->lineEdit->text().isEmpty()){
            if(ui->lineEdit->text().startsWith("http")){
                image = ui->lineEdit->text();
            }
            else
                return;
        }
    }
    //Si on est ici, tous les tests sont passés, on peut montrer l'image
    QPixmap jaquette = ((image.startsWith("http")) ? ToolsManager::getPixmapFromString(image) : QPixmap(image));
    QLabel *labelImage = new QLabel;
    labelImage->setPixmap(jaquette);
    QPushButton *close = new QPushButton("Fermer");
    QVBoxLayout *layoutVertical = new QVBoxLayout;
    layoutVertical->addWidget(labelImage);
    layoutVertical->addWidget(close);
    QDialog *preview = new QDialog(this);
    preview->setLayout(layoutVertical);
    connect(close, SIGNAL(clicked()), preview, SLOT(close()));
    preview->exec();
    delete labelImage;
    delete close;
    delete layoutVertical;
    delete preview;
    return;
}

void ImageUploadDialog::selectImage(){
    image = QFileDialog::getOpenFileName(this, "Choisir une photo", QString(), "Images (*.png *.gif *.jpg *.jpeg)");
    return;
}

bool ImageUploadDialog::testFichier(){
    QFile testImage;
    testImage.setFileName(image);
    return ((image.isEmpty()) ? false : testImage.exists());
}
