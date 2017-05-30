#include "apidialog.h"
#include "ui_apidialog.h"

/**
 * Constructor
 * @brief ApiDialog::ApiDialog
 * @param parent
 */
ApiDialog::ApiDialog(QWidget *parent) : QDialog(parent), ui(new Ui::ApiDialog)
{
    ui->setupUi(this);
    this->setConnectors();
}

/**
 * Destructor
 * @brief ApiDialog::~ApiDialog
 */
ApiDialog::~ApiDialog()
{
    delete ui;
    delete login;
    delete registration;
}

/**
 * Connect signals and slots
 * @brief ApiDialog::setConnectors
 */
void ApiDialog::setConnectors()
{
    this->login = new QSignalMapper();
    this->login->setMapping(ui->buttonLogin, 1);
    connect(ui->pushButtonActionLogin, SIGNAL(clicked()), this->login, SLOT(map()));
    connect(this->login, SIGNAL(mapped(int)), ui->stackedWidget, SLOT(setCurrentIndex(int)));

    this->registration = new QSignalMapper();
    this->registration->setMapping(ui->buttonRegister, 2);
    connect(ui->pushButtonActionRegister, SIGNAL(clicked()), this->registration, SLOT(map()));
    connect(this->registration, SIGNAL(mapped(int)), ui->stackedWidget, SLOT(setCurrentIndex(int)));
}
