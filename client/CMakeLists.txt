cmake_minimum_required(VERSION 3.10)
project(cavoke_client)

set(QT_MAJOR_VERSION 6 CACHE STRING "Qt Major Version (e.g. Qt5/Qt6)")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

find_package(Qt${QT_MAJOR_VERSION}Widgets REQUIRED)
find_package(Qt${QT_MAJOR_VERSION}Quick REQUIRED)
find_package(Qt${QT_MAJOR_VERSION}Network REQUIRED)
find_package(Qt${QT_MAJOR_VERSION}NetworkAuth REQUIRED)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

include_directories(${CMAKE_BINARY_DIR}
        controllers
        models
        views
        #        ../third_party/qtkeychain
        )

# https://api.kde.org/frameworks-api/frameworks-apidocs/frameworks/karchive/html/index.html
find_package(KF5Archive REQUIRED)

#add_compile_definitions(MOCK)
add_compile_definitions(REAL)

aux_source_directory(views VIEWS_DIR)
aux_source_directory(entities ENTITIES_DIR)

add_executable(cavoke_client
        main.cpp
        controllers/cavokeclientcontroller.cpp
        models/cavokeclientmodel.cpp
        models/cavokeqmlgamemodel.cpp
        ${VIEWS_DIR}
        ${ENTITIES_DIR}
        cache_manager.cpp
        network_manager.cpp
        resources/resources.qrc
        AuthenticationManager.cpp)

target_link_libraries(cavoke_client PRIVATE Qt${QT_MAJOR_VERSION}::Widgets)
target_link_libraries(cavoke_client PRIVATE Qt${QT_MAJOR_VERSION}::Quick)
target_link_libraries(cavoke_client PRIVATE Qt${QT_MAJOR_VERSION}::Network)
target_link_libraries(cavoke_client PRIVATE Qt${QT_MAJOR_VERSION}::NetworkAuth)
target_link_libraries(cavoke_client PRIVATE KF5::Archive)

# QtKeychain
#target_link_libraries(cavoke_client PRIVATE qt${QT_MAJOR_VERSION}keychain)
#target_compile_definitions(cavoke_client PRIVATE INCLUDE_OWN_QT_KEYCHAIN QTKEYCHAIN_NO_EXPORT)
