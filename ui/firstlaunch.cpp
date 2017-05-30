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

    ui->setupUi(this);

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
 * Activate XML button
 * @brief FirstLaunch::setXML
 */
void FirstLaunch::setXML(){
    if(ui->buttonXML->isChecked()){
        ui->buttonCloud->setChecked(false);
        ui->buttonSQLite->setChecked(false);
    }
    return;
}

/**
 * Activate Cloud button
 * @brief FirstLaunch::setCloud
 */
void FirstLaunch::setCloud(){
    if(ui->buttonCloud->isChecked()){
        ui->buttonSQLite->setChecked(false);
        ui->buttonXML->setChecked(false);
    }
    return;
}

/**
 * Activate SQLite button
 * @brief FirstLaunch::setSQLite
 */
void FirstLaunch::setSQLite(){
    if(ui->buttonSQLite->isChecked()){
        ui->buttonCloud->setChecked(false);
        ui->buttonXML->setChecked(false);
    }
    return;
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
    connect(ui->buttonXML, SIGNAL(clicked()), this, SLOT(setXML()));
    connect(ui->buttonSQLite, SIGNAL(clicked()), this, SLOT(setSQLite()));
    connect(ui->buttonCloud, SIGNAL(clicked()), this, SLOT(setCloud()));
    connect(ui->buttonGo, SIGNAL(clicked()), this, SLOT(accept()));
}
