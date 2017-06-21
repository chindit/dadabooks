#ifndef STRINGTOOLS_H
#define STRINGTOOLS_H

#include <QString>
#include <QStringList>

class StringTools
{
public:
    static QString stripDeterminants(QString string);
    static bool isUrl(QString string);
};

#endif // STRINGTOOLS_H
