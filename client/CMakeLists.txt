cmake_minimum_required(VERSION 3.16)
project(cavoke_client)

set(QT_MAJOR_VERSION 6 CACHE STRING "Qt Major Version (e.g. Qt5/Qt6)")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

find_package(ECM 5.90.0 REQUIRED CONFIG)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${ECM_MODULE_PATH} )

include(FeatureSummary)
include(ECMAddAppIcon)
include(ECMQtDeclareLoggingCategory)
include(ECMInstallIcons)
include(ECMSetupVersion)
include(KDEInstallDirs)
include(KDECMakeSettings)

find_package(Qt${QT_MAJOR_VERSION} REQUIRED COMPONENTS Widgets Quick Network NetworkAuth)

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

install(TARGETS ${PROJECT_NAME} ${KDE_INSTALL_TARGETS_DEFAULT_ARGS})
configure_file(${CMAKE_SOURCE_DIR}/client/cavoke.desktop
        ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}.desktop)
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}.desktop DESTINATION ${KDE_INSTALL_APPDIR} )
file(GLOB_RECURSE ICON_FILES "${CMAKE_CURRENT_SOURCE_DIR}/resources/packaging/*-apps-cavoke.png")

# Add icon files to the application's source files to have CMake bundle them in the executable.
ecm_add_app_icon(ICONS_SOURCES ICONS ${ICON_FILES})
target_sources(${PROJECT_NAME} PRIVATE ${ICONS_SOURCES})

ecm_install_icons(
        ICONS ${ICON_FILES}
        DESTINATION ${KDE_INSTALL_ICONDIR}
        THEME hicolor
)
