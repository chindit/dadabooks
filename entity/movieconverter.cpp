#include "movieconverter.h"

/**
 * Map a list of Movie to a QList
 * @brief MovieConverter::toMap
 * @param movies
 * @return
 */
QList<QMultiMap<QString, QString> > MovieConverter::toMap(QList<Movie*> movies)
{
    QList<QMultiMap<QString, QString> > result;

    for (Movie* movie : movies) {
        QMultiMap<QString, QString> mappedMovie = MovieConverter::toMap(movie);
        if (!mappedMovie.empty()) {
            result.append(mappedMovie);
        }
    }

    return result;
}

/**
 * Map an object to a list
 * @brief MovieConverter::toMap
 * @param movie
 * @return
 */
QMultiMap<QString, QString> MovieConverter::toMap(Movie* movie)
{
    QMultiMap<QString, QString> mappedMovie;
    mappedMovie.insert("id", QString::number(movie->getId()));
    mappedMovie.insert("title", movie->getTitle());
    mappedMovie.insert("originalTitle", movie->getTitle());
    mappedMovie.insert("year", movie->getYear().toString());
    mappedMovie.insert("director", movie->getDirector());
    mappedMovie.insert("actors", movie->getActors().join(','));
    mappedMovie.insert("synopsis", movie->getSynopsis());
    mappedMovie.insert("duration", QString::number(movie->getDuration()));
    mappedMovie.insert("genders", movie->getGenders().join(','));
    mappedMovie.insert("countries", movie->getCountries().join(','));
    mappedMovie.insert("languages", movie->getLanguages().join(','));
    mappedMovie.insert("subtitles", movie->getSubtitles().join(','));
    mappedMovie.insert("classification", movie->getClassification());
    mappedMovie.insert("comment", movie->getComment());
    mappedMovie.insert("labels", movie->getLabels().join(','));
    mappedMovie.insert("notation", QString::number(movie->getNotation()));
    mappedMovie.insert("cover", movie->getCover());
    mappedMovie.insert("borrowable", (movie->isBorrowable()) ? "1" : "0");
    mappedMovie.insert("borrowed", (movie->isBorrowed()) ? "1" : "0");
    mappedMovie.insert("seen", (movie->isSeen()) ? "1" : "0");
    mappedMovie.insert("file", (movie->hasFile()) ? "1" : "0");
    mappedMovie.insert("fileLocation", movie->getFile());

    return mappedMovie;
}

/**
 * Map a  list of object to a list of entities
 * @brief MovieConverter::fromMap
 * @param data
 * @return
 */
QList<Movie*> MovieConverter::fromMap(QList<QMultiMap<QString, QString> > data)
{
    QList<Movie*> result;


    for (QMultiMap<QString, QString> datum : data) {
        Movie* mappedMovie = MovieConverter::fromMap(datum);
        result.append(mappedMovie);
    }
    return result;
}

/**
 * Map a list to an entity
 * @brief MovieConverter::fromMap
 * @param data
 * @return
 */
Movie* MovieConverter::fromMap(QMultiMap<QString, QString> data)
{
    QMap<QString, QString>::const_iterator i;
    Movie* movie = new Movie();

    for (i = data.cbegin(); i != data.cend(); ++i) {
        if (i.key() == "id") {
            movie->setId(QString(i.value()).toInt());
        }
        if (i.key() == "title") {
            movie->setTitle(i.value());
        }
        if (i.key() == "originalTitle") {
            movie->setOriginalTitle(i.value());
        }
        if (i.key() == "year") {
            movie->setYear(QDate::fromString(i.value()));
        }
        if (i.key() == "director") {
            movie->setDirector(i.value());
        }
        if (i.key() == "actors") {
            movie->setActors(QString(i.value()).split(','));
        }
        if (i.key() == "synopsis") {
            movie->setSynopsis(i.value());
        }
        if (i.key() == "duration") {
            movie->setDuration(QString(i.value()).toInt());
        }
        if (i.key() == "genders") {
            movie->setGenders(QString(i.value()).split(','));
        }
        if (i.key() == "countries") {
            movie->setCountries(QString(i.value()).split(','));
        }
        if (i.key() == "languages") {
            movie->setLanguages(QString(i.value()).split(','));
        }
        if (i.key() == "subtitles") {
            movie->setSubtitles(QString(i.value()).split(','));
        }
        if (i.key() == "classification") {
            movie->setClassification(i.value());
        }
        if (i.key() == "comment") {
            movie->setComment(i.value());
        }
        if (i.key() == "labels") {
            movie->setLabels(QString(i.value()).split(','));
        }
        if (i.key() == "notation") {
            movie->setNotation(QString(i.value()).toInt());
        }
        if (i.key() == "cover") {
            movie->setCover(i.value());
        }
        if (i.key() == "borrowable") {
            movie->setBorrowable(i.value() == "1");
        }
        if (i.key() == "borrowed") {
            movie->setBorrowed(i.value() == "1");
        }
        if (i.key() == "seen") {
            movie->setSeen((i.value() == "1"));
        }
        if (i.key() == "file") {
            movie->setHasFile(i.value() == "1");
        }
        if (i.key() == "fileLocation") {
            movie->setFile(i.value());
        }
    }

    return movie;
}
