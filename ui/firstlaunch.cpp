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
    this->logger = new Logger(this->settings, parent);

    ui->setupUi(this);
    this->updateStorageLabel();

    this->setConnectors();
}

/**
 * Destructor
 *
 * @brief FirstLaunch::~FirstLaunch
 */
FirstLaunch::~FirstLaunch(){
    //delete ui;
    delete logger;
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
 * Connect buttons to actions
 * @brief FirstLaunch::setConnectors
 */
void FirstLaunch::setConnectors()
{
    connect(ui->stackedWidget, SIGNAL(currentChanged(int)), this, SLOT(updateBreadcrumb(int)));

    connect(ui->startButton, SIGNAL(clicked()), this, SLOT(next()));
    connect(ui->nextButton, SIGNAL(clicked()), this, SLOT(next()));
    connect(ui->nextButton2, SIGNAL(clicked()), this, SLOT(checkStorageType()));

    connect(ui->buttonOtherStorage, SIGNAL(clicked()), this, SLOT(selectStorageMethod()));
}

/**
 * Update storage label
 * @brief FirstLaunch::updateStorageLabel
 */
void FirstLaunch::updateStorageLabel()
{
    ui->labelStorageInfo->setText(tr("Votre collection de %s sera stockée dans le répertoire %s")
                                  .arg(ui->comboBoxCollectionType->currentText()).arg(this->currentDir));
}

/**
 * Check if storage type is correctly enabled
 * @brief FirstLaunch::checkStorageType
 */
void FirstLaunch::checkStorageType()
{
    PluginLoader *loader = new PluginLoader(this->logger);
    long formatChecked = 0;
    if (ui->pushButtonXML->isChecked()) {
        if (!loader->hasStoragePluginName("XML")) {
            QMessageBox::warning(this, tr("Plugin not found"), tr("Requested storage method is not available."));
            ui->pushButtonXML->setEnabled(false);
        }
        else {
            formatChecked++;
        }
    }
    if (ui->pushButtonSQLite->isChecked()) {
        if (!loader->hasStoragePluginName("SQLite")) {
            QMessageBox::warning(this, tr("Plugin not found"), tr("Requested storage method is not available."));
            ui->pushButtonSQLite->setEnabled(false);
        }
        else {
            formatChecked++;
        }
    }
    if (ui->pushButtonCloud->isChecked()) {
        if (!loader->hasStoragePluginName("Cloud")) {
            QMessageBox::warning(this, tr("Plugin not found"), tr("Requested storage method is not available."));
            ui->pushButtonCloud->setEnabled(false);
        }
        else {
            formatChecked++;
        }
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

/**
 * Allow user to select an alternate storage method
 * @brief FirstLaunch::selectStorageMethod
 */
void FirstLaunch::selectStorageMethod()
{
    PluginLoader *loader = new PluginLoader(this->logger);
    StorageSelectionDialog *storageSelectionDialog = new StorageSelectionDialog(loader, this);
    storageSelectionDialog->exec();
    // Once it's done, we get selected method
    QString selectedStorageMethod = storageSelectionDialog->getSelectedPlugin();
    if (selectedStorageMethod.length() > 0) {
        // TODO save setting
        this->settings;
        this->next();
    }
    delete storageSelectionDialog;
    delete loader;
}

void FirstLaunch::finish()
{
    // Saving settings
    this->settings->setSetting(Type, ui->comboBoxCollectionType->currentText().toLower());
    this->settings->setSetting(Fichier, this->currentDir);
}
