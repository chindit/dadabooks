cmake_minimum_required(VERSION 3.7)
project(DadaBooks)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_AUTOMOC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)

include_directories( ${CMAKE_BINARY_DIR} )

# We need add -DQT_WIDGETS_LIB when using QtWidgets in Qt 5.
add_definitions(${Qt5Widgets_DEFINITIONS})

# Executables fail to build with Qt 5 in the default configuration
# without -fPIE. We add that here.
set(CMAKE_CXX_FLAGS "${Qt5Widgets_EXECUTABLE_COMPILE_FLAGS}")

set(SOURCE_FILES
        config/config.h
        entity/movie.cpp
        entity/movie.h
        entity/movieconverter.cpp
        entity/movieconverter.h
        images/a_propos.png
        images/add.png
        images/chargement.png
        images/delete.png
        images/edit.png
        images/etiquette.png
        images/etiquette_small.png
        images/fleche_droite.png
        images/fleche_gauche.png
        images/html.png
        images/icone.png
        images/icone_125.jpg
        images/insert_actor.png
        images/insert_genre.png
        images/livre.png
        images/loader.gif
        images/logo_qt.jpg
        images/maj.png
        images/moins.png
        images/nouveau.png
        images/ok.png
        images/open.png
        images/options.png
        images/pdf.png
        images/plus.png
        images/quitter.png
        images/random.png
        images/recent.png
        images/recherche.png
        images/search.png
        plugins/storageplugin.h
        plugins/thirdpartyplugin.h
        tools/exceptioninterface.h
        tools/logger.cpp
        tools/logger.h
        tools/pluginloader.cpp
        tools/pluginloader.h
        tools/settings.cpp
        tools/settings.h
        tools/storage.cpp
        tools/storage.h
        ui/apidialog.cpp
        ui/apidialog.h
        ui/firstlaunch.cpp
        ui/firstlaunch.h
        ui/storageselectiondialog.cpp
        ui/storageselectiondialog.h
        .gitignore
        addauteur.cpp
        addauteur.h
        addauteur.ui
        addbook.cpp
        addbook.h
        addbook.ui
        addediteur.cpp
        addediteur.h
        addediteur.ui
        dadabooks.cpp
        dadabooks.h
        dadabooks.pro
        dadabooks.pro.user
        dadabooks.ui
        editbook.cpp
        editbook.h
        editbook.ui
        images.qrc
        imageuploaddialog.cpp
        imageuploaddialog.h
        imageuploaddialog.ui
        lenddialog.cpp
        lenddialog.h
        lenddialog.ui
        LICENSE
        main.cpp
        PluginTemplate.h
        previewbook.cpp
        previewbook.h
        previewbook.ui
        README.md
        searchdialog.cpp
        searchdialog.h
        searchdialog.ui
        settingsdialog.cpp
        settingsdialog.h
        settingsdialog.ui
        settingsmanager.cpp
        settingsmanager.h
        sitemanager.cpp
        sitemanager.h
        sqlmanager.cpp
        sqlmanager.h
        toolsmanager.cpp
        toolsmanager.h
        updater.cpp
        updater.h
        xmlmanager.cpp
        xmlmanager.h
        dadabooks.ui
        addbook.ui
        editbook.ui
        addauteur.ui
        addediteur.ui
        previewbook.ui
        settingsdialog.ui
        ui/firstlaunch.ui
        imageuploaddialog.ui
        lenddialog.ui
        searchdialog.ui
        ui/apidialog.ui
        ui/storageselectiondialog.ui)

add_executable(DadaBooks ${SOURCE_FILES} ${SOURCES_UIS_H})

find_package( Qt5 REQUIRED Core Gui Network)

find_package(Qt5Core)
find_package(Qt5Network)
find_package(Qt5Widgets)
find_package(Qt5Xml)
find_package(Qt5Sql)
find_package(Qt5PrintSupport)

get_target_property(QtCore_location Qt5::Core LOCATION)
target_link_libraries(DadaBooks Qt5::Core Qt5::Network Qt5::Widgets Qt5::Xml Qt5::Sql Qt5::PrintSupport)
