cmake_minimum_required(VERSION 3.10)
project(cavoke_client)

option(QT_MAJOR_VERSION "Qt Major Version (e.g. Qt5/Qt6)" 6)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

find_package(Qt${QT_MAJOR_VERSION}Widgets REQUIRED)
find_package(Qt${QT_MAJOR_VERSION}Quick REQUIRED)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

include_directories(${CMAKE_BINARY_DIR})

# https://api.kde.org/frameworks-api/frameworks-apidocs/frameworks/karchive/html/index.html
find_package(KF5Archive REQUIRED)

add_executable(cavoke_client
        main.cpp
        cavokeclientcontroller.cpp
        cavokeclientmodel.cpp
        cavokeclientview.cpp
        cavokeqmlgamemodel.cpp
        tictactoelogic.cpp
        cache_manager.cpp)

target_link_libraries(cavoke_client Qt${QT_MAJOR_VERSION}::Widgets)
target_link_libraries(cavoke_client Qt${QT_MAJOR_VERSION}::Quick)
target_link_libraries(cavoke_client KF5::Archive)
