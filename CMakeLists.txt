cmake_minimum_required(VERSION 3.13)
set(TARGET_NAME dadabooks)

project(${TARGET_NAME})

# Windows only  Comment these two lines on Linux
set (CMAKE_PREFIX_PATH "D:\\Qt\\5.14.1\\mingw73_64")
include_directories(SYSTEM D:\\Qt\\5.14.1\\mingw73_64\\include)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

# Check the build type and ask the user to set concrete one
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE RelWithDebInfo)
    message(WARNING "CMAKE_BUILD_TYPE is not set, forcing to RelWithDebInfo")
endif()

# Set compiler flags
if(${CMAKE_CXX_COMPILER_ID} MATCHES "GNU" OR ${CMAKE_CXX_COMPILER_ID} MATCHES "Clang")
    set(CMAKE_CXX_FLAGS "-Wall -Wextra")
    set(CMAKE_CXX_FLAGS_DEBUG "-O0 -g3")
    set(CMAKE_CXX_FLAGS_RELEASE "-O3")
    set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "-O3 -g3")
    set(CMAKE_CXX_FLAGS_MINSIZEREL "-Os")
endif()

find_package(Qt5Core REQUIRED)
find_package(Qt5Gui REQUIRED)
find_package(Qt5Widgets REQUIRED)
find_package(Qt5Network REQUIRED)
find_package(Qt5PrintSupport REQUIRED)
find_package(Qt5Sql REQUIRED)
find_package(Qt5Xml REQUIRED)

include_directories(
        ${CMAKE_CURRENT_BINARY_DIR}
        ${CMAKE_CURRENT_SOURCE_DIR}
)

# Instruct CMake to run moc automatically when needed
set(CMAKE_AUTOMOC ON)

# Source files
set(SOURCES
        main.cpp
        dadabooks.cpp
        settingsmanager.cpp
        addbook.cpp
        sitemanager.cpp
        editbook.cpp
        addauteur.cpp
        addediteur.cpp
        xmlmanager.cpp
        sqlmanager.cpp
        previewbook.cpp
        settingsdialog.cpp
        firstlaunch.cpp
        toolsmanager.cpp
        imageuploaddialog.cpp
        lenddialog.cpp
        updater.cpp
        searchdialog.cpp
        )

# User interface files
set(FORMS
        dadabooks.ui
        addbook.ui
        editbook.ui
        addauteur.ui
        addediteur.ui
        previewbook.ui
        settingsdialog.ui
        firstlaunch.ui
        imageuploaddialog.ui
        lenddialog.ui
        searchdialog.ui
        )

# Resource files
set(RESOURCES
        images.qrc
        )

# Shared libraries
set(LIBRARIES
        Qt5::Widgets
        Qt5::Network
        Qt5::Sql
        Qt5::Xml
        )

# Generate additional sources with MOC and UIC
qt5_wrap_ui(UIC_SOURCES ${FORMS})
qt5_add_resources(RCC_SOURCES ${RESOURCES})

# Set target
add_executable(${TARGET_NAME} ${SOURCES} ${HEADERS} ${UIC_SOURCES} ${RCC_SOURCES})

target_link_libraries(${TARGET_NAME} Qt5::Core)
target_link_libraries(${TARGET_NAME} Qt5::Gui)
target_link_libraries(${TARGET_NAME} Qt5::Widgets)
target_link_libraries(${TARGET_NAME} Qt5::Network)
target_link_libraries(${TARGET_NAME} Qt5::PrintSupport)
target_link_libraries(${TARGET_NAME} Qt5::Sql)
target_link_libraries(${TARGET_NAME} Qt5::Xml)

set_property(TARGET ${TARGET_NAME} PROPERTY CXX_STANDARD 20)