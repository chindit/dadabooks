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
    QString dirName;
    QString extensionChoice = (isXML) ? tr("Fichiers XML (*.xml)") : tr("Bases de données (*.db *.sqlite)");
    dirName = QFileDialog::getSaveFileName(this, tr("Emplacement de la base de données"), QDir::homePath(), extensionChoice);

    // Handling «no choice» (not allowed)
    QFileInfo location = QFileInfo(dirName);
    if (location.isDir()) {

        if (!dirName.endsWith(QDir::separator())) {
            dirName.append(QDir::separator());
        }
        dirName.append("dadabooks.");
        dirName.append((isXML) ? "xml" : "db");
    } else if (!location.isFile()) {
        // Should be OK, it will be created
    } else {
        QMessageBox::information(this, tr("Choisissez un dossier"), tr("Merci de choisir l'emplacement du fichier de base de données"));
        return this->getDirName(isXML);
    }

    // Adding possibly missing extension for storage file
    if (!isXML && !dirName.endsWith(".db") && !dirName.endsWith(".sqlite")) {
        dirName.append(".db");
    } else if (isXML && !dirName.endsWith(".xml")) {
        dirName.append(".xml");
    }

    if (!location.exists()) {
        QString dirPath = location.absoluteDir().path();
        location = QFileInfo(dirPath);
    }

    if (!location.isWritable()) {
        QMessageBox::information(this, tr("Emplacement invalide"), tr("Impossible d'écrire dans ce répertoire.  Veuillez changer les droits"
                                                                      " d'accès ou choisir un autre répertoire"));
        return this->getDirName(isXML);
    }

    return dirName;
}

/**
 * Connect buttons to actions
 * @brief FirstLaunch::setConnectors
 * @internal
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
 * @internal
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
 * @internal
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
 * @internal
 */
void FirstLaunch::updateBreadcrumb(int current)
{
    ui->labelBreadcrumbIndex->setText(QString::number(current+1)+"/"+QString::number(ui->stackedWidget->count()));
}

/**
 * Change to next page in stackedWidget
 * @brief FirstLaunch::next
 * @internal
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
    this->ui->labelStorageInfo->setText(this->currentFile);
}

/**
 * Allow user to select an alternate storage method
 * @brief FirstLaunch::selectStorageMethod
 * @internal
 */
void FirstLaunch::selectStorageMethod()
{
    PluginLoader *loader = new PluginLoader(this->logger);
    StorageSelectionDialog *storageSelectionDialog = new StorageSelectionDialog(loader, this);
    int result = storageSelectionDialog->exec();
    if (result == QDialog::Rejected) {
        return;
    }
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

/**
 * Get current parameters from the UI
 * @brief FirstLaunch::getStorageConfigFromUI
 * @return
 */
QList<StorageConfig> FirstLaunch::getStorageConfigFromUI()
{
    PluginLoader *loader = new PluginLoader(this->logger);
    QList<StorageConfig> defaultConfig = loader->getStoragePlugin(this->storageEngineId)->getActiveParameters();

    if (this->ui->pushButtonCloud->isChecked()) {
        // TODO Get cloud parameters
    } else {
        defaultConfig.clear();
        // Get file
        StorageConfig file;
        file.id = "file";
        file.value = QVariant((this->currentFile.isEmpty()) ? this->getDirName(this->storageEngineId.contains("XML", Qt::CaseSensitive)) : this->currentFile);
        // Get type
        StorageConfig type;
        type.id = "type";
        type.value = QVariant(this->getSelectedCollectionType());

        this->storageEngineConfiguration.append(file);
        this->storageEngineConfiguration.append(type);
    }

    return this->storageEngineConfiguration;
}

/**
 * Get selected collection type in Checkbox
 * @brief FirstLaunch::getSelectedCollectionType
 * @return
 */
Collection FirstLaunch::getSelectedCollectionType()
{
    QString typeColl = ui->comboBoxCollectionType->currentText().toLower();

    for (auto &type : collectionNames) {
        if (type.second == typeColl) {
            return type.first;
        }
    }
    this->logger->error(tr("Unable to save settings: collection type of %1 is not recognized").arg(typeColl), QMap<QString, QString>());
    return None;
}

/**
 * Intecept the end of process and save data
 * @brief FirstLaunch::finish
 * @internal
 */
void FirstLaunch::finish()
{
    // Get storage engine configuration if not set
    if (this->storageEngineConfiguration.empty()) {
        this->storageEngineConfiguration = this->getStorageConfigFromUI();
    }

    // Reading last config options
    if (ui->checkBoxSubdir->isChecked()) {
        QFileInfo attemptedDir = QFileInfo(this->currentFile);
        QString basedir = attemptedDir.dir().path() + QDir::separator() + "dadabooks";
        QDir requestedDir = QDir(basedir);
        if (!requestedDir.exists()) {
            bool result = requestedDir.mkdir(basedir);
            if (!result) {
                this->logger->error(tr("Unable to create «dadabooks» dir in %1").arg(basedir));
                return;
            }
        }
        this->currentFile = basedir + QDir::separator() + attemptedDir.fileName();
    }

    // Try to create storage
    PluginLoader *loader = new PluginLoader(this->logger);
    if (!loader->getStoragePlugin(this->storageEngineId)->create(this->storageEngineConfiguration)) {
        this->logger->error(tr("Unable to initiate storage.  Error is following: ") +
                               loader->getStoragePlugin(this->storageEngineId)->getLastError(), QMap<QString, QString>());
        int response = QMessageBox::question(this, tr("Unable to create storage"), tr("Storage engine was unable"
                                                                                      " to create it's configuration.  Do"
                                                                                      " you want to change your parameters ?"),
                                             QMessageBox::Yes | QMessageBox::No);
        if (response == QMessageBox::No) {
            this->close();
            return;
        } else {
            return;
        }
    }
    // Saving settings
    collection.type = this->getSelectedCollectionType();
    collection.storageEngine = this->storageEngineId;
    collection.useDefaultStorageSettings = false;
    collection.storageEngineConfig = (this->storageEngineConfiguration.count() > 0) ?
                this->storageEngineConfiguration
              : loader->getStoragePlugin(this->storageEngineId)->getActiveParameters();
    QDateTime *now = new QDateTime();
    collection.id = QString(collectionNames.find(collection.type)->second) + QString("_") + now->toSecsSinceEpoch();
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

/**
 * Return newly created collection
 * @brief FirstLaunch::getCollection
 * @return
 */
CollectionStorageSettings FirstLaunch::getCollection()
{
    return this->collection;
}
