#include "stringtools.h"

QString StringTools::stripDeterminants(QString string)
{
    QStringList determinants;
    // FR
    determinants << "L'" << "La " << "Le " << "Les " << "Un " << "Une ";
    // ES
    determinants << "Al " << "El " << "Un " << "Una " << "Las " << "Los ";
    // EN
    determinants << "The ";

    foreach(const QString &elem, determinants){
        if(string.startsWith(elem, Qt::CaseInsensitive)){
            string = string.right(string.size()-elem.size());
            string.append(" (");
            string.append(elem.trimmed());
            string.append(")");
            string = string.trimmed();
            string.replace(0, 1, string.at(0).toUpper());
        }
    }
    return string.trimmed();
}
