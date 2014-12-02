#ifndef DADABOOKS_H
#define DADABOOKS_H

#include <QCheckBox>
#include <QGridLayout>
#include <QListWidget>
#include <QMainWindow>
#include <QProcess>
#include <QTextEdit>

#include "addbook.h"
#include "editbook.h"
#include "firstlaunch.h"
#include "previewbook.h"
#include "settingsdialog.h"
#include "settingsmanager.h"
#include "sitemanager.h"
#include "sqlmanager.h"
#include "xmlmanager.h"

#define VERSION "0.5.0"

namespace Ui {
class DadaBooks;
}

class DadaBooks : public QMainWindow
{
    Q_OBJECT

public:
    explicit DadaBooks(QWidget *parent = 0);
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


private:
    void intabPreview(int id);

    AddBook *insAddBook;
    Ui::DadaBooks *ui;
    EditBook *insEditBook;
    PreviewBook *insPreviewBook;
    SettingsDialog *insSettingsDialog;
    SettingsManager *insSettingsManager;
    SiteManager *insSiteManager;
    SqlManager *insSqlManager;
    XmlManager *insXmlManager;

    int idOngletEdit;
};

#endif // DADABOOKS_H
