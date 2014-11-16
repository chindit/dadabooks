#ifndef SQL_H
#define SQL_H

#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlRecord>
#include <QtSql/QSqlError>
#include <QDebug>

#include "settingsmanager.h"

class SqlManager
{
public:
    SqlManager();
    ~SqlManager();
    QSqlQuery query(QString req);
    QList<QMultiMap<QString, QString> > convertToXML(QSqlQuery requete, bool films=false);

private:
    void connect();
    void disconnect();
    QSqlDatabase instance;
    SettingsManager *insManager;
};

#endif // SQL_H
