#ifndef IMAGEUPLOADDIALOG_H
#define IMAGEUPLOADDIALOG_H

#include <QDialog>
#include <QFile>

#include "toolsmanager.h"

namespace Ui {
class ImageUploadDialog;
}

class ImageUploadDialog : public QDialog{
    Q_OBJECT

public:
    explicit ImageUploadDialog(QWidget *parent = 0);
    ~ImageUploadDialog();
    QString getImage();

private slots:
    void selectImage();
    void testImage();
    bool testFichier();

private:
    Ui::ImageUploadDialog *ui;
    QString image;
};

#endif // IMAGEUPLOADDIALOG_H
