cmake_minimum_required(VERSION 3.10)
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

#if(WIN32)
#    set(CMAKE_WIN32_EXECUTABLE ON)
#elseif(APPLE)
#    set(CMAKE_MACOSX_BUNDLE ON)
#endif()
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
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}.desktop DESTINATION ${DATA_INSTALL_DIR}/applications )
#set(target ${PROJECT_NAME})
#set_target_properties(${target} PROPERTIES AUTORCC_OPTIONS "--compress;9")

# QtKeychain
#target_link_libraries(cavoke_client PRIVATE qt${QT_MAJOR_VERSION}keychain)
#target_compile_definitions(cavoke_client PRIVATE INCLUDE_OWN_QT_KEYCHAIN QTKEYCHAIN_NO_EXPORT)

#if(UNIX AND NOT APPLE)
#    install(TARGETS ${target} RUNTIME DESTINATION bin)
#elseif(WIN32)
#    install(TARGETS ${target} RUNTIME DESTINATION bin)
#elseif(APPLE)
#    set(CMAKE_MACOSX_BUNDLE ON)
#    set(CMAKE_MACOSX_RPATH ON)
#    install(TARGETS ${target} BUNDLE DESTINATION .
#            RUNTIME DESTINATION .)
#    set_source_files_properties(resources/icon.icns PROPERTIES MACOSX_PACKAGE_LOCATION Resources)
#    set_target_properties(${target}
#            PROPERTIES
#            MACOSX_BUNDLE_BUNDLE_NAME "${PROJECT_NAME}"
#            MACOSX_BUNDLE_INFO_STRING "${PROJECT_NAME} Copyright (c) 2019 Example@Example.com"
#            MACOSX_BUNDLE_ICON_FILE Icon.icns
#            MACOSX_BUNDLE_GUI_IDENTIFIER "${PROJECT_NAME}"
#            MACOSX_BUNDLE_LONG_VERSION_STRING "${PROJECT_VERSION}"
#            MACOSX_BUNDLE_SHORT_VERSION_STRING "${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}"
#            MACOSX_BUNDLE_BUNDLE_VERSION "${PROJECT_VERSION}"
#            MACOSX_BUNDLE_COPYRIGHT "Copyright (c) 2019 Example@Example.com"
#            )
#endif()
#
#set(desktop.path applications)
#set(desktop.files resources/packaging/cavoke.desktop)
#set(icon.path icons/hicolor/64x64/apps)
#set(icon.files resources/packaging/icon64.png)
#set(iconsvg.path icons/hicolor/scalable/apps)
#set(iconsvg.files resources/packaging/icon.svg)
#foreach(items IN ITEMS desktop icon iconsvg)
#    install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/${${items}.files}
#            DESTINATION share/${${items}.path}
#            PERMISSIONS OWNER_READ OWNER_WRITE GROUP_READ WORLD_READ)
#endforeach()
## Components:
#if(CMAKE_BUILD_TYPE_UPPER MATCHES "^(DEBUG|RELWITHDEBINFO)$")
#    set(CPACK_STRIP_FILES FALSE)
#else()
#    set(CPACK_STRIP_FILES TRUE)
#endif()
#
#include(${CMAKE_CURRENT_LIST_DIR}/Packaging.cmake)