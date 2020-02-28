#ifndef NV_LIVRE_H
#define NV_LIVRE_H

#include <QtWidgets/QAbstractButton>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QDialog>

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
    void searchInternet(QString titre, QString site, QString langue);
    void canceled();

private slots:
    void addInternetBook();
    void addManualBook();
    void emitClose();

private:
    Ui::AddBook *ui;
    EditBook *insEditBook;
    SiteManager *insSiteManager;
    SettingsManager *insSettingsManager;
};

#endif // NV_LIVRE_H
