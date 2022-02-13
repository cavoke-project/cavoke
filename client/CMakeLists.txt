cmake_minimum_required(VERSION 3.10)
project(client)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

if (NOT QT_MAJOR_VERSION)
    set(QT_MAJOR_VERSION 6)
endif ()

find_package(Qt${QT_MAJOR_VERSION}Widgets REQUIRED)
find_package(Qt${QT_MAJOR_VERSION}Quick REQUIRED)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

include_directories(${CMAKE_BINARY_DIR}
        controllers
        models
        views)

# https://api.kde.org/frameworks-api/frameworks-apidocs/frameworks/karchive/html/index.html
find_package(KF5Archive REQUIRED)

add_executable(client
        main.cpp
        controllers/cavokeclientcontroller.cpp
        controllers/startcontroller.cpp
        models/cavokeclientmodel.cpp
        models/cavokeqmlgamemodel.cpp
        models/startmodel.cpp
        views/cavokeclientview.cpp
        views/startview.cpp
        tictactoelogic.cpp
        cache_manager.cpp
        )

target_link_libraries(client Qt${QT_MAJOR_VERSION}::Widgets)
target_link_libraries(client Qt${QT_MAJOR_VERSION}::Quick)
target_link_libraries(client KF5::Archive)
