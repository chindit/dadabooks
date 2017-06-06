#include "movie.h"

Movie::Movie()
{

}

int Movie::getId()
{
    return this->id;
}

Movie* Movie::setId(int id)
{
    this->id = id;
    return this;
}

QString Movie::getTitle()
{
    return this->title;
}

Movie* Movie::setTitle(QString title)
{
    this->title = title;
    return this;
}

QString Movie::getOriginalTitle()
{
    return this->originalTitle;
}

Movie* Movie::setOriginalTitle(QString originalTitle)
{
    this->originalTitle = originalTitle;
    return this;
}

QDate Movie::getYear()
{
    return this->year;
}

Movie* Movie::setYear(QDate year)
{
    this->year = year;
    return this;
}

QString Movie::getDirector()
{
    return this->director;
}

Movie* Movie::setDirector(QString director)
{
    this->director = director;
    return this;
}

QStringList Movie::getActors()
{
    return this->actors;
}

Movie* Movie::setActors(QStringList actors)
{
    this->actors = actors;
    return this;
}

Movie* Movie::addActor(QString actor)
{
    this->actors.append(actor);
    return this;
}

Movie* Movie::removeActor(QString actor)
{
    for (int i=0; i<this->actors.size(); i++) {
        if (this->actors.at(i) == actor) {
            this->actors.removeAt(i);
            break;
        }
    }
    return this;
}

QString Movie::getSynopsis()
{
    return this->synopsis;
}

Movie* Movie::setSynopsis(QString synopsis)
{
    this->synopsis = synopsis;
    return this;
}

int Movie::getDuration()
{
    return this->duration;
}

Movie* Movie::setDuration(int duration) {
    this->duration = duration;
    return this;
}

QStringList Movie::getGenders()
{
    return this->genders;
}

Movie* Movie::setGenders(QStringList genders)
{
    this->genders = genders;
    return this;
}

Movie* Movie::addGender(QString gender)
{
    this->genders.append(gender);
    return this;
}

Movie* Movie::removeGender(QString gender)
{
    for (int i=0; i<this->genders.size(); i++) {
        if (this->genders.at(i) == gender) {
            this->genders.removeAt(i);
            break;
        }
    }
    return this;
}

QStringList Movie::getCountries()
{
    return this->countries;
}

Movie* Movie::setCountries(QStringList countries)
{
    this->countries = countries;
    return this;
}

Movie* Movie::addCountry(QString country)
{
    this->countries.append(country);
    return this;
}

Movie* Movie::removeCountry(QString country)
{
    for (int i=0; i<this->countries.size(); i++) {
        if (this->countries.at(i) == country) {
            this->countries.removeAt(i);
            break;
        }
    }
    return this;
}

QStringList Movie::getLanguages()
{
    return this->languages;
}

Movie* Movie::setLanguages(QStringList languages)
{
    this->languages = languages;
    return this;
}

Movie* Movie::addLanguage(QString language)
{
    this->languages.append(language);
    return this;
}

Movie* Movie::removeLanguage(QString language)
{
    for (int i=0; i<this->languages.size(); i++) {
        if (this->languages.at(i) == language) {
            this->languages.removeAt(i);
            break;
        }
    }
    return this;
}

QStringList Movie::getSubtitles()
{
    return this->subtitles;
}

Movie* Movie::setSubtitles(QStringList subtitles)
{
    this->subtitles = subtitles;
    return this;
}

Movie* Movie::addSubtitle(QString subtitle)
{
    this->subtitles.append(subtitle);
    return this;
}

Movie* Movie::removeSubtitle(QString subtitle)
{
    for (int i=0; i<this->subtitles.size(); i++) {
        if (this->subtitles.at(i) == subtitle) {
            this->subtitles.removeAt(i);
            break;
        }
    }
    return this;
}

QString Movie::getClassification()
{
    return this->classification;
}

Movie* Movie::setClassification(QString classification)
{
    this->classification = classification;
    return this;
}

QString Movie::getComment()
{
    return this->comment;
}

Movie* Movie::setComment(QString comment)
{
    this->comment = comment;
    return this;
}

QStringList Movie::getLabels()
{
    return this->labels;
}

Movie* Movie::setLabels(QStringList labels)
{
    this->labels = labels;
    return this;
}

Movie* Movie::addLabel(QString label)
{
    this->labels.append(label);
    return this;
}

Movie* Movie::removeLabel(QString label)
{
    for (int i=0; i<this->labels.size(); i++) {
        if (this->labels.at(i) == label) {
            this->labels.removeAt(i);
            break;
        }
    }
    return this;
}

int Movie::getNotation()
{
    return this->notation;
}

Movie* Movie::setNotation(int notation)
{
    this->notation = notation;
    return this;
}

QString Movie::getCover()
{
    return this->cover;
}

Movie* Movie::setCover(QString cover)
{
    this->cover = cover;
    return this;
}

bool Movie::isBorrowable()
{
    return this->borrowable;
}

Movie* Movie::setBorrowable(bool borrowable)
{
    this->borrowable = borrowable;
    return this;
}

bool Movie::isBorrowed()
{
    return this->borrowed;
}

Movie* Movie::setBorrowed(bool borrowed)
{
    this->borrowed = borrowed;
    return this;
}

bool Movie::isSeen()
{
    return this->seen;
}

Movie* Movie::setSeen(bool seen)
{
    this->seen = seen;
    return this;
}

bool Movie::hasFile()
{
    return this->file;
}

Movie* Movie::setHasFile(bool hasFile)
{
    this->file = hasFile;

    return this;
}

QString Movie::getFile()
{
    return this->fileLocation;
}

Movie* Movie::setFile(QString file)
{
    this->fileLocation = file;
    this->file = (file.length() > 0);
    return this;
}
