#ifndef XMLMANAGER_H
#define XMLMANAGER_H

#include <QtCore/QDate>
#include <QtCore/QFile>
#include <QtWidgets/QFileDialog>
#include <QtCore/QObject>
#include <QtCore/QTextStream>
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
    void exportCurrentAsGCStar(QList< QMultiMap<QString, QString> > base);
    int getIdByTitle(QString titre);
    QList<QMultiMap<QString, QString> > getElementsByLabel(QString label);
    QStringList getListEtiquettes();
    
signals:
    
public slots:

private:
    SettingsManager *insManager;
    
};

#endif // XMLMANAGER_H
