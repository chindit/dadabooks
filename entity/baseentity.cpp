#include "entity/baseentity.h"

BaseEntity* BaseEntity::setId(int id)
{
    this->id = id;
    return this;
}

int BaseEntity::getId()
{
    return this->id;
}

BaseEntity* BaseEntity::setTitle(QString title)
{
    this->title = title;
    return this;
}

QString BaseEntity::getTitle()
{
    return this->title;
}
