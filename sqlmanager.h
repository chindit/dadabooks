#ifndef SQL_H
#define SQL_H

#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlRecord>
#include <QtSql/QSqlError>
#include <QDebug>
#include <QElapsedTimer>


#include "settingsmanager.h"
#include "toolsmanager.h"

class SqlManager
{
public:
    SqlManager();
    ~SqlManager();
    QSqlQuery query(QString req);
    QList<QMultiMap<QString, QString> > convertToXML(QSqlQuery requete);
    QStringList getListEtiquettes();

private:
    void connect();
    void disconnect();
    QSqlDatabase instance;
    SettingsManager *insManager;
    QElapsedTimer name;
};

#endif // SQL_H
