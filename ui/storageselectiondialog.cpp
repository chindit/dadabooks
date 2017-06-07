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
 * Hydrate list of available StoragePlugins
 * @brief StorageSelectionDialog::hydrateComboBox
 */
void StorageSelectionDialog::hydrateComboBox()
{
    for (StoragePlugin* plugin : this->pluginLoader->getStoragePluginList()) {
        ui->comboBoxStoragePlugins->addItem(plugin->getName());
    }
}
