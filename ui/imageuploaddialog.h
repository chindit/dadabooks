#ifndef IMAGEUPLOADDIALOG_H
#define IMAGEUPLOADDIALOG_H

#include <QDialog>
#include <QDialogButtonBox>
#include <QFile>

#include "tools/logger.h"
#include "tools/networkmanager.h"
#include "tools/stringtools.h"

#include "ui_imageuploaddialog.h"

namespace Ui {
class ImageUploadDialog;
}

class ImageUploadDialog : public QDialog{
    Q_OBJECT

public:
    explicit ImageUploadDialog(Logger *logger, QWidget *parent = 0);
    ~ImageUploadDialog();
    QString getImage();

private slots:
    void selectImage();
    void updateImage(QString file);

private:
    void setConnectors();

    Ui::ImageUploadDialog *ui;
    NetworkManager *network;
    QPixmap *picture;
    QString image;
};

#endif // IMAGEUPLOADDIALOG_H
