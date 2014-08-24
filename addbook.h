#ifndef NV_LIVRE_H
#define NV_LIVRE_H

#include <QDialog>
#include <QAbstractButton>

#include "editbook.h"
#include "sitemanager.h"

namespace Ui {
class AddBook;
}

class AddBook : public QDialog
{
    Q_OBJECT
    
public:
    explicit AddBook(QWidget *parent = 0);
    ~AddBook();

signals:
    void searchInternet(QString titre, QString site);

private slots:
    void addInternetBook();
    void addManualBook();

private:
    Ui::AddBook *ui;
    EditBook *insEditBook;
    SiteManager *insSiteManager;
};

#endif // NV_LIVRE_H
