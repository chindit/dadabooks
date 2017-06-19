#ifndef ABSTRACTENTITY_H
#define ABSTRACTENTITY_H

#include <QString>

class BaseEntity
{
public:
    BaseEntity *setId(int id) ;
    int getId();
    QString getTitle();
    BaseEntity *setTitle(QString title);

private:
    int id;
    QString title;
};

#endif // ABSTRACTENTITY_H