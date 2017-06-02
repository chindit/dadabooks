#ifndef MOVIEENTITY_H
#define MOVIEENTITY_H

#include <QDate>
#include <QString>
#include <QTime>

class MovieEntity
{
public:
    MovieEntity();

    int getId();
    MovieEntity *setId(int id);
    QString getTitle();
    MovieEntity *setTitle(QString title);
    QString getOriginalTitle();
    MovieEntity *setOriginalTitle(QString title);
    QDate getYear();
    MovieEntity *setYear(QDate year);
    QString getDirector();
    MovieEntity *setDirector(QString director);
    QStringList getActors();
    MovieEntity *setActors(QStringList actors);
    MovieEntity *addActor(QString actor);
    MovieEntity *removeActor(QString actor);
    QString getSynopsis();
    MovieEntity *setSynopsis(QString synopsis);
    QTime getDuration();
    MovieEntity *setDuration(QTime duration);
    QStringList getGenders();
    MovieEntity *setGenders(QStringList genders);
    MovieEntity *addGender(QString gender);
    MovieEntity *removeGender(QString gender);
    QStringList getCountries();
    MovieEntity *setCountries(QStringList countries);
    MovieEntity *addCountry(QString country);
    MovieEntity *removeCountry(QString country);
    QStringList getLanguages();
    MovieEntity *setLanguages(QStringList languages);
    MovieEntity *addLanguage(QString language);
    MovieEntity *removeLanguage(QString language);
    QStringList getSubtitles();
    MovieEntity *setSubtitles(QStringList subtitles);
    MovieEntity *addSubtitle(QString subtitle);
    MovieEntity *removeSubtitle(QString subtitle);
    QString getClassification();
    MovieEntity *setClassification(QString classification);
    QString getComment();
    MovieEntity *setComment(QString comment);
    QStringList getLabels();
    MovieEntity *setLabels(QStringList labels);
    MovieEntity *addLabel(QString label);
    MovieEntity *removeLabel(QString label);
    int getNotation();
    MovieEntity *setNotation(int notation);
    QString getCover();
    MovieEntity *setCover(QString cover);
    bool isBorrowable();
    MovieEntity *setBorrowable(bool borrowable);
    bool isBorrowed();
    MovieEntity *setBorrowed(bool borrowed);
    bool isSeen();
    MovieEntity *setSeen(bool seen);
    bool hasFile();
    QString getFile();
    MovieEntity *setFile(QString file);

private:
    int id;
    QString title;
    QString originalTitle;
    QDate year;
    QString director;
    QStringList actors;
    QString synopsis;
    QTime duration;
    QStringList genders;
    QStringList countries;
    QStringList languages;
    QStringList subtitles;
    QString classification;
    QString comment;
    QStringList labels;
    int notation;
    QString cover;
    bool borrowable;
    bool borrowed;
    bool seen;
    bool file;
    QString fileLocation;
};

#endif // MOVIEENTITY_H
