#ifndef MOVIEENTITY_H
#define MOVIEENTITY_H

#include <QDate>
#include <QString>
#include <QTime>

class Movie
{
public:
    Movie();

    int getId();
    Movie *setId(int id);
    QString getTitle();
    Movie *setTitle(QString title);
    QString getOriginalTitle();
    Movie *setOriginalTitle(QString title);
    QDate getYear();
    Movie *setYear(QDate year);
    QString getDirector();
    Movie *setDirector(QString director);
    QStringList getActors();
    Movie *setActors(QStringList actors);
    Movie *addActor(QString actor);
    Movie *removeActor(QString actor);
    QString getSynopsis();
    Movie *setSynopsis(QString synopsis);
    int getDuration();
    Movie *setDuration(int duration);
    QStringList getGenders();
    Movie *setGenders(QStringList genders);
    Movie *addGender(QString gender);
    Movie *removeGender(QString gender);
    QStringList getCountries();
    Movie *setCountries(QStringList countries);
    Movie *addCountry(QString country);
    Movie *removeCountry(QString country);
    QStringList getLanguages();
    Movie *setLanguages(QStringList languages);
    Movie *addLanguage(QString language);
    Movie *removeLanguage(QString language);
    QStringList getSubtitles();
    Movie *setSubtitles(QStringList subtitles);
    Movie *addSubtitle(QString subtitle);
    Movie *removeSubtitle(QString subtitle);
    QString getClassification();
    Movie *setClassification(QString classification);
    QString getComment();
    Movie *setComment(QString comment);
    QStringList getLabels();
    Movie *setLabels(QStringList labels);
    Movie *addLabel(QString label);
    Movie *removeLabel(QString label);
    int getNotation();
    Movie *setNotation(int notation);
    QString getCover();
    Movie *setCover(QString cover);
    bool isBorrowable();
    Movie *setBorrowable(bool borrowable);
    bool isBorrowed();
    Movie *setBorrowed(bool borrowed);
    bool isSeen();
    Movie *setSeen(bool seen);
    bool hasFile();
    Movie *setHasFile(bool hasFile);
    QString getFile();
    Movie *setFile(QString file);

private:
    int id;
    QString title;
    QString originalTitle;
    QDate year;
    QString director;
    QStringList actors;
    QString synopsis;
    int duration;
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
