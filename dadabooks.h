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

#define VERSION "0.0.1"

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
    void rechercheInternet(QString requete, QString site);
    void getBook(QString id);
    void updateOnglet(int id);
    void setListeLivres();
    void activatePreview(int id=0, bool search=false, bool idOk=false);
    void preparePreview();
    void closeTab(int tab);
    void deleteLivre(int id);
    void editLivre(int id);
    void makeSearch();
    void prepareSearchView(int row);
    void openFile();
    void about();
    void openNewColl();
    void showEtiquette(const int &id);
    void openTagList(QString tag);
    void expandIntabContent(bool status);


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
    QHBoxLayout *layoutEtiquettesLivres;
    QLabel *titre_fen, *id1, *id2, *titre, *titre2, *isbn, *isbn2, *coauteurs, *coauteurs2, *synopsis, *couverture, *couverture2, *pages, *pages2, *edition, *edition2, *langue, *langue2, *classement, *classement2, *exemplaires, *exemplaires2, *commentaire, *annee, *annee2, *auteur, *auteur2, *editeur, *editeur2, *note1, *note2, *ebook2, *ebook3, *listeEtiquettes;
    QCheckBox *empruntable, *prete, *lu, *ebook;
    QPushButton *buttonEdit, *buttonDelete, *buttonListWidgets;
    QTextEdit *synopsis2, *commentaire2;
    QSignalMapper *mapperDelete, *mapperEdit;
    QListWidget *listWidget, *listGenresWidget;

    int idOngletEdit;
    QString nomSiteRecherche;
};

#endif // DADABOOKS_H
