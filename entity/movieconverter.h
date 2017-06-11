#ifndef MOVIECONVERTER_H
#define MOVIECONVERTER_H

#include <QList>
#include <QMultiMap>
#include "entity/movie.h"

class MovieConverter
{
public:
    static QList<QMultiMap<QString, QString> > toMap(QList<Movie*> movies);
    static QMultiMap<QString, QString> toMap(Movie* movie);
    static QList<Movie*> fromMap(QList<QMultiMap<QString, QString> > data);
    static Movie* fromMap(QMultiMap<QString, QString> data);
};

#endif // MOVIECONVERTER_H
