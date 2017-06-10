#include "storageselectiondialog.h"
#include "ui_storageselectiondialog.h"

/**
 * Constructor
 * @brief StorageSelectionDialog::StorageSelectionDialog
 * @param pluginLoader
 * @param parent
 */
StorageSelectionDialog::StorageSelectionDialog(PluginLoader *pluginLoader, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::StorageSelectionDialog)
{
    this->pluginLoader = pluginLoader;
    ui->setupUi(this);
    connect(ui->comboBoxStoragePlugins, SIGNAL(currentIndexChanged()), this, SLOT(updateParameters()));
    this->hydrateComboBox();
}

/**
 * Destructor
 * @brief StorageSelectionDialog::~StorageSelectionDialog
 */
StorageSelectionDialog::~StorageSelectionDialog()
{
    delete ui;
}

/**
 * Get current selected StoragePlugin
 * @brief StorageSelectionDialog::getSelectedPlugin
 * @return
 */
QString StorageSelectionDialog::getSelectedPlugin()
{
    return (this->pluginLoader->hasStoragePluginName(ui->comboBoxStoragePlugins->currentText())) ?
                this->pluginLoader->getStoragePluginByName(ui->comboBoxStoragePlugins->currentText())->getUID() :
                QString();
}

/**
 * Get custom parameters for storage engine
 * @brief StorageSelectionDialog::getPluginConfig
 * @return
 */
QList<StorageConfig> StorageSelectionDialog::getPluginConfig()
{
    QList<StorageConfig> parameters = this->pluginLoader->getStoragePlugin(this->getSelectedPlugin())->getDefaultParameters();
    QList<QLineEdit *> allLineEdits = ui->parametersLayout->findChildren<QLineEdit *>();
    for (int i=0; i<allLineEdits.count(); i++) {
        QString id = allLineEdits.at(i)->property("id").toString();
        for (int j=0; j<parameters.count(); j++) {
            if (parameters.at(j).id == id) {
                parameters[j].value = QVariant(allLineEdits.at(i)->text());
                break;
            }
        }
    }
    return parameters;
}

/**
 * Hydrate list of available StoragePlugins
 * @brief StorageSelectionDialog::hydrateComboBox
 */
void StorageSelectionDialog::hydrateComboBox()
{
    for (StoragePlugin* plugin : this->pluginLoader->getStoragePluginList()) {
        ui->comboBoxStoragePlugins->addItem(plugin->getName());
    }
}

/**
 * Create widgets for current storage method
 * @brief StorageSelectionDialog::updateParameters
 * @param plugin
 */
void StorageSelectionDialog::updateParameters()
{
    if (this->getSelectedPlugin().isEmpty()) {
        return;
    }
    // Clean previous widgets
    while (ui->parametersLayout->count() > 0) {
        ui->parametersLayout->removeRow(0);
    }
    // Update with new values
    QList<StorageConfig> parameters = this->pluginLoader->getStoragePlugin(this->getSelectedPlugin())->getDefaultParameters();
    for (StorageConfig parameter : parameters) {
        QLabel *label = new QLabel(parameter.description);
        QLineEdit *lineEdit = new QLineEdit(parameter.value.toString());
        lineEdit->setProperty("id", parameter.id);
        // Adding to layout
        ui->parametersLayout->addRow(label, lineEdit);
    }
}
