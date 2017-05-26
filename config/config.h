#ifndef CONFIG_H
#define CONFIG_H

#include <boost/assign/list_of.hpp>
#include <boost/unordered_map.hpp>

// Settings enumeration
enum Setting { AutoAuteur, AutoEditeur, Sqlite, MariaDB, DBHost, DBUser, DBPass, DBBase, Xml, Fichier, Initialized, OpenInTab, DownPics, Type, Empruntable };

// Settings variables
const boost::unordered_map<Setting, const char*> settingsNames = boost::assign::map_list_of
    (AutoAuteur, "AutoAuteur")
    (AutoEditeur, "AutoEditeur")
    (Sqlite, "useSQLite")
    (MariaDB, "useMySQL")
    (Xml, "useXML")
    (Fichier, "fichierActuel")
    (DBHost, "MariaDB_host")
    (DBUser, "MariaDB_user")
    (DBPass, "MariaDB_password")
    (DBBase, "MariaDB_base")
    (Initialized, "isInitialized")
    (OpenInTab, "ouvrirOngletPrincipal")
    (DownPics, "telechargerImages")
    (Type, "typeCollection")
    (Empruntable, "isEmpruntable");

// Collection type handled by DadaBooks
enum Collection { Book, Movie };

// Collection variables
const boost::unordered_map<Collection, const char*> collectionNames = boost::assign::map_list_of
    (Book, "livres")
    (Movie, "films");

#endif // CONFIG_H
