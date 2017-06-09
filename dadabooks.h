#ifndef DADABOOKS_H
#define DADABOOKS_H

#include <QCheckBox>
#include <QGridLayout>
#include <QListWidget>
#include <QMainWindow>
#include <QMenuBar>
#include <QMovie>
#include <QProcess>
#include <QTextEdit>

#include <time.h>

#include "addbook.h"
#include "editbook.h"
#include "ui/firstlaunch.h"
#include "lenddialog.h"
#include "previewbook.h"
#include "searchdialog.h"
#include "settingsdialog.h"
#include "tools/settings.h"
#include "settingsmanager.h"
#include "sitemanager.h"
#include "sqlmanager.h"
#include "updater.h"
#include "xmlmanager.h"
#include "tools/storage.h"

#define VERSION "0.8.3"

namespace Ui {
class DadaBooks;
}

class DadaBooks : public QMainWindow
{
    Q_OBJECT

public:
    explicit DadaBooks(QWidget *parent = 0);
    void init();
    ~DadaBooks();

private slots:
    void rechercheInternet(QString requete, QString site, QString langue);
    void getBook(QString id, QString site);
    void updateOnglet(int id);
    void setListeLivres();
    void activatePreview(int id=0, bool search=false, bool idOk=false);
    void preparePreview();
    void closeTab(int tab);
    void deleteLivre(int id);
    void deleteLivre(); //Surcharge pour appel
    void editLivre(int id);
    void editLivre(); //Surcharge pour appel
    void makeSearch();
    void prepareSearchView(int row);
    void openFile();
    void about();
    void openNewColl();
    void showEtiquette(const int &id);
    void showEtiquette(const QString nom);
    void openTagList(QString tag);
    void expandIntabContent(bool status);
    void selectRandom();
    void exportAsHTML();
    void exportAsPDF();
    void setEditCanceled();
    void showAddBook();
    void showInitStacked();
    void prepareLendDialog();
    void lendItem(QString nom, QString email);
    void returnItem(int idLend, int idItem);


private:
    void setConnectors();

    Settings *settings;
    Storage *storage;

    void intabPreview(int id);
    int getCurrentItemID();

    AddBook *insAddBook;
    Ui::DadaBooks *ui;
    EditBook *insEditBook;
    LendDialog *insLendDialog;
    PreviewBook *insPreviewBook;
    SearchDialog *insSearchDialog;
    SettingsDialog *insSettingsDialog;
    SettingsManager *insSettingsManager;
    SiteManager *insSiteManager;
    SqlManager *insSqlManager;
    Updater *insUpdater;
    XmlManager *insXmlManager;
    QMovie *movieLoading;

    int idOngletEdit;
    bool isCalling;
};

#endif // DADABOOKS_H
