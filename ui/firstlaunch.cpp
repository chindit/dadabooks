#include "firstlaunch.h"
#include "ui_firstlaunch.h"

/**
 * Constructor
 *
 * @brief FirstLaunch::FirstLaunch
 * @param parent
 * @param settings
 */
FirstLaunch::FirstLaunch(QWidget *parent, Settings *settings) : QDialog(parent), ui(new Ui::FirstLaunch){
    this->settings = settings;
    this->currentDir = QStandardPaths::writableLocation(QStandardPaths::HomeLocation);

    ui->setupUi(this);
    this->setupUi();

    this->setConnectors();
}

/**
 * Destructor
 *
 * @brief FirstLaunch::~FirstLaunch
 */
FirstLaunch::~FirstLaunch(){
    delete ui;
    // We don't need to delete *settings as the pointer is handled by sender.
}

/**
 * Get user location for storage file
 *
 * @brief FirstLaunch::getDirName
 * @param isXML
 * @return
 */
QString FirstLaunch::getDirName(bool isXML){
    QMessageBox::information(this, tr("Choisissez un dossier"), tr("Merci de choisir l'emplacement du fichier de base de données"));
    QString dirName;
    QString extensionChoice = (isXML) ? tr("Bases de données (*.db *.sqlite)") : tr("Fichiers XML (*.xml)");
    dirName = QFileDialog::getSaveFileName(this, tr("Emplacement de la base de données"), QDir::homePath(), extensionChoice);

    // Handling «no choice» (not allowed)
    QFileInfo location = QFileInfo(dirName);
    if (location.isDir()) {
        return this->getDirName(isXML);
    }

    // Adding possibly missing extension for storage file
    if (!isXML && !dirName.endsWith(".db") && !dirName.endsWith(".sqlite")) {
        dirName.append(".db");
    } else if (isXML && !dirName.endsWith(".xml")) {
        dirName.append(".xml");
    }

    return dirName;
}

/**
 * Handle User choise about DB
 * @brief FirstLaunch::accept
 */
void FirstLaunch::accept(){
    // Close modal
    this->close();

    // Cloud
    if(ui->buttonCloud->isChecked()){
        ApiDialog *apiDialogWindow = new ApiDialog(this);
        apiDialogWindow->exec();
        delete apiDialogWindow;
    }
    // File-based storage
    else {
        settings->setSetting(Sqlite, ui->buttonSQLite->isChecked());
        settings->setSetting(Xml, ui->buttonXML->isChecked());
        QString fileName = this->getDirName();
        settings->setSetting(Fichier, fileName);
    }

    // Handling collection type
    bool collectionFound = false;
    Collection detectedCollection;
    for ( auto it = collectionNames.begin(); it != collectionNames.end(); ++it ) {
        if (it->second == ui->comboBox_Type->currentText().toLower()) {
            detectedCollection = it->first;
            collectionFound = true;
            break;
        }
    }
    if (collectionFound) {
        settings->setSetting(Type, detectedCollection);
    } else {
        // Logic exception.  This code shouldn't be reached
        QMessageBox::critical(this, tr("Aucun type de collection choisi"), tr("Aucun type de collection n'a été choisi.  Il est donc impossible de continuer"));
    }
}

/**
 * Connect buttons to actions
 * @brief FirstLaunch::setConnectors
 */
void FirstLaunch::setConnectors()
{
    connect(ui->stackedWidget, SIGNAL(currentChanged(int)), this, SLOT(updateBreadcrumb(int)));

    connect(ui->startButton, SIGNAL(clicked()), this, SLOT(next()));
    connect(ui->nextButton, SIGNAL(clicked()), this, SLOT(next()));
    connect(ui->nextButton2, SIGNAL(clicked()), this, SLOT(checkStorageType()));
    /*connect(ui->buttonXML, SIGNAL(clicked()), this, SLOT(setXML()));
    connect(ui->buttonSQLite, SIGNAL(clicked()), this, SLOT(setSQLite()));
    connect(ui->buttonCloud, SIGNAL(clicked()), this, SLOT(setCloud()));
    connect(ui->buttonGo, SIGNAL(clicked()), this, SLOT(accept()));*/
}

/**
 * Initialize some custom UI settings
 * @brief FirstLaunch::setupUi
 */
void FirstLaunch::setupUi()
{
    // TODO labelStorageInfo
}

/**
 * Check if storage type is correctly enabled
 * @brief FirstLaunch::checkStorageType
 */
void FirstLaunch::checkStorageType()
{
    long formatChecked = 0;
    if (ui->pushButtonXML->isChecked()) {
        formatChecked++;
    }
    if (ui->pushButtonSQLite->isChecked()) {
        formatChecked++;
    }
    if (ui->pushButtonCloud->isChecked()) {
        formatChecked++;
    }

    if (formatChecked == 1) {
        this->next();
        return;
    }
    if (formatChecked == 0) {
        QMessageBox::information(this, tr("Pas de méthode de stockage"), tr("Vous devez choisir une méthode de stockage pour votre collection"));
    } else {
        QMessageBox::information(this, tr("Trop de méthodes"), tr("Vous ne pouvez sélectionner qu'une seule méthode de stockage"));
    }
}

/**
 * Update breadcrumb to reflect current position
 * @brief FirstLaunch::updateBreadcrumb
 * @param current
 */
void FirstLaunch::updateBreadcrumb(int current)
{
    ui->labelBreadcrumbIndex->setText(QString::number(current)+"/"+QString::number(ui->stackedWidget->count()));
}

/**
 * Change to next page in stackedWidget
 * @brief FirstLaunch::next
 */
void FirstLaunch::next()
{
    ui->stackedWidget->setCurrentIndex(ui->stackedWidget->currentIndex()+1);
}

/**
 * Alias of getDirName without first argument
 * @brief FirstLaunch::getStorageDir
 */
void FirstLaunch::getStorageDir()
{
    this->getDirName(ui->pushButtonXML->isChecked());
}

void FirstLaunch::finish()
{
    // Saving settings
    // TODO check default return
    Collection = collectionNames.find(ui->comboBoxCollectionType->currentText().toLower())->first;
    this->settings->setSetting(Type, ui->comboBoxCollectionType->currentText().toLower());
    this->settings->setSetting(Fichier, this->currentDir);
    this->sett
}
