#ifndef STRINGTOOLS_H
#define STRINGTOOLS_H

#include <QFile>
#include <QString>
#include <QStringList>
#include <QUrl>

class StringTools
{
public:
    static QString stripDeterminants(QString string);
    static bool isUrl(QString string);
    static bool isFile(QString string);
};

#endif // STRINGTOOLS_H
