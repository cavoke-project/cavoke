cmake_minimum_required(VERSION 3.10)
project(cavoke_client)

set(QT_MAJOR_VERSION 6 CACHE STRING "Qt Major Version (e.g. Qt5/Qt6)")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

find_package(Qt${QT_MAJOR_VERSION}Widgets REQUIRED)
find_package(Qt${QT_MAJOR_VERSION}Quick REQUIRED)
find_package(Qt${QT_MAJOR_VERSION}Network REQUIRED)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

include_directories(${CMAKE_BINARY_DIR}
        controllers
        models
        views)

# https://api.kde.org/frameworks-api/frameworks-apidocs/frameworks/karchive/html/index.html
find_package(KF5Archive REQUIRED)

#add_compile_definitions(MOCK)
add_compile_definitions(REAL)

add_executable(cavoke_client
        main.cpp
        controllers/cavokeclientcontroller.cpp
        models/cavokeclientmodel.cpp
        models/cavokeqmlgamemodel.cpp
        views/testwindowview.cpp
        views/startview.cpp
        views/joingameview.cpp
        views/creategameview.cpp
        views/settingsview.cpp
        views/gameslistview.cpp
        views/protoroomview.cpp
        entities/gameinfo.cpp
        entities/sessioninfo.cpp
        entities/validationresult.cpp
        cache_manager.cpp
        network_manager.cpp
        entities/player.cpp
        resources/resources.qrc)

target_link_libraries(cavoke_client Qt${QT_MAJOR_VERSION}::Widgets)
target_link_libraries(cavoke_client Qt${QT_MAJOR_VERSION}::Quick)
target_link_libraries(cavoke_client Qt${QT_MAJOR_VERSION}::Network)
target_link_libraries(cavoke_client KF5::Archive)
