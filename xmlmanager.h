#ifndef XMLMANAGER_H
#define XMLMANAGER_H

#include <QDate>
#include <QFile>
#include <QFileDialog>
#include <QObject>
#include <QTextStream>
#include <QtXml/QDomDocument>
#include <QtXml/QDomElement>
#include <iterator>

#include "settingsmanager.h"

class XmlManager : public QObject
{
    Q_OBJECT
public:
    explicit XmlManager(QObject *parent = 0);
    ~XmlManager();
    QList<QMultiMap<QString, QString> > readBase(int id = 0);
    QMultiMap<QString, QString> getBook(int id);
    bool writeBase(QList<QMultiMap<QString,QString> > base);
    void addBook(QMultiMap<QString, QString> livre);
    void deleteBook(int id);
    QList<QMultiMap<QString, QString> > makeSearch(QStringList mots);
    void exportCurrentAsGCStar();
    int getIdByTitle(QString titre);
    
signals:
    
public slots:

private:
    SettingsManager *insManager;
    
};

#endif // XMLMANAGER_H
