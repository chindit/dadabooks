#include "imageuploaddialog.h"

/**
 * Constructor
 * @brief ImageUploadDialog::ImageUploadDialog
 * @param parent
 */
ImageUploadDialog::ImageUploadDialog(Logger *logger, QWidget *parent) : QDialog(parent), ui(new Ui::ImageUploadDialog){
    ui->setupUi(this);
    this->image = QString();
    this->network = new NetworkManager(logger);
    this->picture = new QPixmap();
    this->setConnectors();

    // Disable «Finish» button by default
    this->ui->buttonBox->button(QDialogButtonBox::AcceptRole)->setEnabled(false);
}

/**
 * Destructor
 * @brief ImageUploadDialog::~ImageUploadDialog
 */
ImageUploadDialog::~ImageUploadDialog(){
    delete network;
    delete picture;
    delete ui;
}

/**
 * Create connections for UI elements
 * @brief ImageUploadDialog::setConnectors
 */
void ImageUploadDialog::setConnectors()
{
    connect(ui->pushButtonChoose, SIGNAL(clicked()), this, SLOT(selectImage()));
    connect(ui->lineEditUrl, SIGNAL(textEdited(QString)), this, SLOT(testFile(QString)));
}

/**
 * Return selected picture, if valid
 * @brief ImageUploadDialog::getImage
 * @return
 */
QString ImageUploadDialog::getImage(){
    return this->image;
}

/**
 * Update file in preview
 * @brief ImageUploadDialog::updateImage
 * @param file
 */
void ImageUploadDialog::updateImage(QString file)
{
    if (StringTools::isUrl(file)) {
        this->picture->loadFromData(this->network->downloadFile(file));
    } else if (StringTools::isFile(file)) {
        this->picture->load(file);
    } else {
        this->picture = QPixmap();
    }

    if (!this->picture->isNull()) {
        ui->labelPicture->setPixmap(this->picture);
        this->image = file;
    } else {
        ui->labelPicture->setText(tr("Invalid file"));
    }
}

/**
 * Select a picture from the User's computer
 * @brief ImageUploadDialog::selectImage
 */
void ImageUploadDialog::selectImage(){
    QString selectedPicture = QFileDialog::getOpenFileName(this, tr("Choose a picture"), QString(), "Images (*.png *.gif *.jpg *.jpeg)");
    this->updateImage(selectedPicture);
    return;
}
