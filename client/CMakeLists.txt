cmake_minimum_required(VERSION 3.10)
project(client)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

if (USE_QT5)
    find_package(Qt5Widgets REQUIRED)
    find_package(Qt5Quick REQUIRED)
else(USE_QT5)
    find_package(Qt6Widgets REQUIRED)
    find_package(Qt6Quick REQUIRED)
endif(USE_QT5)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

include_directories(${CMAKE_BINARY_DIR})

# https://api.kde.org/frameworks-api/frameworks-apidocs/frameworks/karchive/html/index.html
find_package(KF5Archive REQUIRED)

add_executable(client-prototype
        main.cpp
        cavokeclientcontroller.cpp
        cavokeclientmodel.cpp
        cavokeclientview.cpp
        cavokeqmlgamemodel.cpp
        tictactoelogic.cpp
        cache_manager.cpp)

if (USE_QT5)
    target_link_libraries(client-prototype Qt5::Widgets)
    target_link_libraries(client-prototype Qt5::Quick)
else(USE_QT5)
    target_link_libraries(client-prototype Qt6::Widgets)
    target_link_libraries(client-prototype Qt6::Quick)
endif(USE_QT5)

target_link_libraries(client-prototype KF5::Archive)

