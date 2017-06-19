#include "additemdialog.h"

/**
 * Constuctor
 * @brief AddItemDialog::AddItemDialog
 * @param loader
 * @param parent
 */
AddItemDialog::AddItemDialog(PluginLoader *loader, QWidget *parent) : QDialog(parent), ui(new Ui::AddItemDialog){
    ui->setupUi(this);
    this->loader = loader;
    this->initPluginList();
    this->setModal(true);
}

/**
 * @brief AddItemDialog::~AddItemDialog Destructor
 */
AddItemDialog::~AddItemDialog(){
    delete ui;
}

/**
 * @brief AddItemDialog::initPluginList Init plugin list in QComboBox
 */
void AddItemDialog::initPluginList()
{
    QList<ThirdPartyPlugin*> thirdPartyPlugins = this->loader->getThirdPartyPluginList();
    QStringList detectedLanguages;

    for (int i=0; i<thirdPartyPlugins.count(); i++) {
        this->ui->comboBox_recherche->insertItem(i, thirdPartyPlugins.at(i)->getName(), QVariant(thirdPartyPlugins.at(i)->getUID()));
        if (!detectedLanguages.contains(thirdPartyPlugins.at(i)->getLanguage().toUpper())) {
            detectedLanguages.append(thirdPartyPlugins.at(i)->getLanguage().toUpper());
            this->ui->comboBoxLangue->insertItem(0, thirdPartyPlugins.at(i)->getLanguage().toUpper());
        }
    }

    this->ui->layoutInternet->setEnabled(thirdPartyPlugins.count() == 0);
    this->ui->layoutLanguage->setEnabled(thirdPartyPlugins.count() == 0);
}

/**
 * @brief AddItemDialog::searchInternet
 */
void AddItemDialog::searchThirdParty(){
    if(ui->lineEdit_recherche->text().isEmpty()){
        QMessageBox::warning(this, tr("Chaîne vide"), tr("Impossible de démarrer la recherche, vous n'avez spécifié aucun terme"));
        return;
    }
    if(ui->comboBox_recherche->currentText().isEmpty()){
        QMessageBox::warning(this, tr("Pas de site"), tr("Vous n'avez spécifié aucun site sur lequel effectuer la recherche"));
        return;
    }
    //Si on est ici, c'est que tout est OK.
    emit searchInternet(ui->lineEdit_recherche->text(), ui->comboBox_recherche->currentText(), ((ui->checkBoxLangue->isChecked()) ? ui->comboBoxLangue->currentText() : ""));
}

/**
 * @brief AddItemDialog::addManualItem
 */
void AddItemDialog::addManualItem(){
    emit addManual();
    this->close();
    return;
}
