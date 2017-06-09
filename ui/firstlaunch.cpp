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
    this->currentFile = QStandardPaths::writableLocation(QStandardPaths::HomeLocation);
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
    connect(ui->buttonFinish, SIGNAL(clicked()), this, SLOT(finish()));

    connect(ui->buttonOtherStorage, SIGNAL(clicked()), this, SLOT(selectStorageMethod()));
    connect(ui->buttonChangeDirectory, SIGNAL(clicked()), this, SLOT(getStorageDir()));
}

/**
 * Update storage label
 * @brief FirstLaunch::updateStorageLabel
 */
void FirstLaunch::updateStorageLabel()
{
    QFileInfo storagePath = QFileInfo(this->currentFile);
    ui->labelStorageInfo->setText(tr("Votre collection de <b>%1</b> sera stockée dans le répertoire <em>%2</em>")
                                  .arg(ui->comboBoxCollectionType->currentText()).arg(storagePath.absoluteDir().absolutePath()));
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
            this->storageEngineId = loader->getStoragePluginByName("XML")->getUID();
        }
    }
    if (ui->pushButtonSQLite->isChecked()) {
        if (!loader->hasStoragePluginName("SQLite")) {
            QMessageBox::warning(this, tr("Plugin not found"), tr("Requested storage method is not available."));
            ui->pushButtonSQLite->setEnabled(false);
        }
        else {
            formatChecked++;
            this->storageEngineId = loader->getStoragePluginByName("SQLite")->getUID();
        }
    }
    if (ui->pushButtonCloud->isChecked()) {
        if (!loader->hasStoragePluginName("Cloud")) {
            QMessageBox::warning(this, tr("Plugin not found"), tr("Requested storage method is not available."));
            ui->pushButtonCloud->setEnabled(false);
        }
        else {
            formatChecked++;
            this->storageEngineId = loader->getStoragePluginByName("Cloud")->getUID();
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
    ui->labelBreadcrumbIndex->setText(QString::number(current+1)+"/"+QString::number(ui->stackedWidget->count()));
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
    this->currentFile = this->getDirName(ui->pushButtonXML->isChecked());
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
        this->storageEngineId = selectedStorageMethod;
        this->storageEngineConfiguration = storageSelectionDialog->getPluginConfig();
        this->next();
    }
    delete storageSelectionDialog;
    delete loader;
}

void FirstLaunch::finish()
{
    // Try to create storage
    PluginLoader *loader = new PluginLoader(this->logger);
    if (!loader->getStoragePlugin(this->storageEngineId)->create(this->storageEngineConfiguration)) {
        this->logger->error(tr("Unable to initiate storage.  Error is following: ") +
                               loader->getStoragePlugin(this->storageEngineId)->getLastError(), QMap<QString, QString>());
        int response = QMessageBox::question(this, tr("Unable to create storage"), tr("Storage engine was unable"
                                                                                      "to create it's configuration.  Do"
                                                                                      "you want to change your parameters ?"),
                                             QMessageBox::Yes | QMessageBox::No);
        if (response == QMessageBox::No) {
            this->close();
            return;
        } else {
            return;
        }
    }
    // Saving settings
    CollectionStorageSettings collection;
    collection.type = ui->comboBoxCollectionType->currentText().toLower();
    collection.storageEngine = this->storageEngineId;
    collection.useDefaultStorageSettings = false;
    collection.storageEngineConfig = loader->getStoragePlugin(this->storageEngineId)->getActiveParameters();
    QDateTime *now = new QDateTime();
    collection.id = collection.type + "_" + now->toSecsSinceEpoch();
    this->settings->storeCollection(collection);

    // Checking initialization
    if (!this->settings->getSetting(Initialized).toBool()) {
        this->settings->setSetting(Initialized, true);
    }
    // Default collection
    if (this->ui->checkBoxDefault->isChecked() || this->settings->getSetting(DefaultCollection).toString().isEmpty()) {
        this->settings->setSetting(DefaultCollection, collection.id);
    }
}
