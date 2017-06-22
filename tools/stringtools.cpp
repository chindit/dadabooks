#include "stringtools.h"

/**
 * Move determinants to the end of the string
 * @brief StringTools::stripDeterminants
 * @param string
 * @return
 */
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

/**
 * Check if URL is valid with strict requirements
 * @brief NetworkManager::isURLValid
 * @param url
 * @return bool
 */
bool StringTools::isUrl(QString string)
{
    QUrl urlObject = QUrl(string, QUrl::StrictMode);
    return urlObject.isValid();
}

/**
 * Check if given path is a valid filename
 * @brief StringTools::isFile
 * @param string
 * @return bool
 */
bool StringTools::isFile(QString string)
{
    QFile file = QFile(string);
    return file.exists() && file.isReadable();
}
