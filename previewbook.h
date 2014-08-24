#ifndef BOOK_PREVIEW_H
#define BOOK_PREVIEW_H

#include <QDialog>
#include <QMultiMap>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QProgressBar>
#include <QMessageBox>

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
    void bookSelected(QString id);
    
private slots:
    void detectClic(int row, int column);

private:
    Ui::PreviewBook *ui;
    QList<QMultiMap<QString, QString> > tableau;
};

#endif // BOOK_PREVIEW_H
