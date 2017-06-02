#include "movieentity.h"

MovieEntity::MovieEntity()
{

}

int MovieEntity::getId()
{
    return this->id;
}

MovieEntity* MovieEntity::setId(int id)
{
    this->id = id;
    return this;
}

QString MovieEntity::getTitle()
{
    return this->title;
}

MovieEntity* MovieEntity::setTitle(QString title)
{
    this->title = title;
    return this;
}

QString MovieEntity::getOriginalTitle()
{
    return this->originalTitle;
}

MovieEntity* MovieEntity::setOriginalTitle(QString originalTitle)
{
    this->originalTitle = originalTitle;
    return this;
}

QDate MovieEntity::getYear()
{
    return this->year;
}

MovieEntity* MovieEntity::setYear(QDate year)
{
    this->year = year;
    return this;
}

QString MovieEntity::getDirector()
{
    return this->director;
}

MovieEntity* MovieEntity::setDirector(QString director)
{
    this->director = director;
    return this;
}

QStringList MovieEntity::getActors()
{
    return this->actors;
}

MovieEntity* MovieEntity::setActors(QStringList actors)
{
    this->actors = actors;
    return this;
}

MovieEntity* MovieEntity::addActor(QString actor)
{
    this->actors.append(actor);
    return this;
}

MovieEntity* MovieEntity::removeActor(QString actor)
{
    for (int i=0; i<this->actors.size(); i++) {
        if (this->actors.at(i) == actor) {
            this->actors.removeAt(i);
            break;
        }
    }
    return this;
}

QString MovieEntity::getSynopsis()
{
    return this->synopsis;
}

MovieEntity* MovieEntity::setSynopsis(QString synopsis)
{
    this->synopsis = synopsis;
    return this;
}

QTime MovieEntity::getDuration()
{
    return this->duration;
}

MovieEntity* MovieEntity::setDuration(QTime duration) {
    this->duration = duration;
    return this;
}

QStringList MovieEntity::getGenders()
{
    return this->genders;
}

MovieEntity* MovieEntity::setGenders(QStringList genders)
{
    this->genders = genders;
    return this;
}

MovieEntity* MovieEntity::addGender(QString gender)
{
    this->genders.append(gender);
    return this;
}

MovieEntity* MovieEntity::removeGender(QString gender)
{
    for (int i=0; i<this->genders.size(); i++) {
        if (this->genders.at(i) == gender) {
            this->genders.removeAt(i);
            break;
        }
    }
    return this;
}

QStringList MovieEntity::getCountries()
{
    return this->countries;
}

MovieEntity* MovieEntity::setCountries(QStringList countries)
{
    this->countries = countries;
    return this;
}

MovieEntity* MovieEntity::addCountry(QString country)
{
    this->countries.append(country);
    return this;
}

MovieEntity* MovieEntity::removeCountry(QString country)
{
    for (int i=0; i<this->countries.size(); i++) {
        if (this->countries.at(i) == country) {
            this->countries.removeAt(i);
            break;
        }
    }
    return this;
}

QStringList MovieEntity::getLanguages()
{
    return this->languages;
}

MovieEntity* MovieEntity::setLanguages(QStringList languages)
{
    this->languages = languages;
    return this;
}

MovieEntity* MovieEntity::addLanguage(QString language)
{
    this->languages.append(language);
    return this;
}

MovieEntity* MovieEntity::removeLanguage(QString language)
{
    for (int i=0; i<this->languages.size(); i++) {
        if (this->languages.at(i) == language) {
            this->languages.removeAt(i);
            break;
        }
    }
    return this;
}

QStringList MovieEntity::getSubtitles()
{
    return this->subtitles;
}

MovieEntity* MovieEntity::setSubtitles(QStringList subtitles)
{
    this->subtitles = subtitles;
    return this;
}

MovieEntity* MovieEntity::addSubtitle(QString subtitle)
{
    this->subtitles.append(subtitle);
    return this;
}

MovieEntity* MovieEntity::removeSubtitle(QString subtitle)
{
    for (int i=0; i<this->subtitles.size(); i++) {
        if (this->subtitles.at(i) == subtitle) {
            this->subtitles.removeAt(i);
            break;
        }
    }
    return this;
}

QString MovieEntity::getClassification()
{
    return this->classification;
}

MovieEntity* MovieEntity::setClassification(QString classification)
{
    this->classification = classification;
    return this;
}

QString MovieEntity::getComment()
{
    return this->comment;
}

MovieEntity* MovieEntity::setComment(QString comment)
{
    this->comment = comment;
    return this;
}

QStringList MovieEntity::getLabels()
{
    return this->labels;
}

MovieEntity* MovieEntity::setLabels(QStringList labels)
{
    this->labels = labels;
    return this;
}

MovieEntity* MovieEntity::addLabel(QString label)
{
    this->labels.append(label);
    return this;
}

MovieEntity* MovieEntity::removeLabel(QString label)
{
    for (int i=0; i<this->labels.size(); i++) {
        if (this->labels.at(i) == label) {
            this->labels.removeAt(i);
            break;
        }
    }
    return this;
}

int MovieEntity::getNotation()
{
    return this->notation;
}

MovieEntity* MovieEntity::setNotation(int notation)
{
    this->notation = notation;
    return this;
}

QString MovieEntity::getCover()
{
    return this->cover;
}

MovieEntity* MovieEntity::setCover(QString cover)
{
    this->cover = cover;
    return this;
}

bool MovieEntity::isBorrowable()
{
    return this->borrowable;
}

MovieEntity* MovieEntity::setBorrowable(bool borrowable)
{
    this->borrowable = borrowable;
    return this;
}

bool MovieEntity::isBorrowed()
{
    return this->borrowed;
}

MovieEntity* MovieEntity::setBorrowed(bool borrowed)
{
    this->borrowed = borrowed;
    return this;
}

bool MovieEntity::isSeen()
{
    return this->seen;
}

MovieEntity* MovieEntity::setSeen(bool seen)
{
    this->seen = seen;
    return this;
}

bool MovieEntity::hasFile()
{
    return this->file;
}

QString MovieEntity::getFile()
{
    return this->fileLocation;
}

MovieEntity* MovieEntity::setFile(QString file)
{
    this->fileLocation = file;
    this->file = (file.length() > 0);
    return this;
}
