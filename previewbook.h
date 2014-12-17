#ifndef BOOK_PREVIEW_H
#define BOOK_PREVIEW_H

#include <QDialog>
#include <QMultiMap>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QProgressBar>
#include <QMessageBox>

#include "settingsmanager.h"

namespace Ui {
class PreviewBook;
}

class PreviewBook : public QDialog
{
    Q_OBJECT
    
public:
    explicit PreviewBook(QWidget *parent = 0);
    ~PreviewBook();
    void setTable(QList< QMultiMap<QString,QString> > elem);

signals:
    void bookSelected(QString id, QString site);
    void canceled();
    
private slots:
    void detectClic(int row, int column);
    void emitClose();

private:
    Ui::PreviewBook *ui;
    SettingsManager *insSettingsManager;
    QList<QMultiMap<QString, QString> > tableau;
};

#endif // BOOK_PREVIEW_H
