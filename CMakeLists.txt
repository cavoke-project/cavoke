cmake_minimum_required(VERSION 3.10)
project(Cavoke
        VERSION 1.1.0
        DESCRIPTION "A Platform for creating and hosting multiplayer turn-based board games"
        HOMEPAGE_URL "https://cavoke.wlko.me"
        LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

option(BUILD_ALL "Build all subprojects (usually disabled for ci)" ON)

option(BUILD_SERVER "Enable building server" OFF)
option(BUILD_CLIENT "Enable building client" OFF)
option(BUILD_GAMES "Enable building games" OFF)

# Add local drogon
if (BUILD_ALL OR BUILD_SERVER OR BUILD_GAMES)
    option(USE_EXTERNAL_DROGON "Whether to use external drogon" OFF)
    if (USE_EXTERNAL_DROGON)
        message("Using external Drogon...")
        find_package(Drogon 1.7.3 REQUIRED)
    else ()
        message("Using Drogon from submodules...")
        if (NOT EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/third_party/drogon/CMakeLists.txt)
            message(SEND_ERROR "Unable to add a dependency locally!\nHave you tried running:  git submodule update --init --recursive")
        endif ()
        add_subdirectory(third_party/drogon)
    endif ()

    option(USE_EXTERNAL_JWT "Whether to use external jwt-cpp" OFF)
    if (USE_EXTERNAL_JWT)
        message("Using external jwt-cpp...")
        # https://github.com/Thalhammer/jwt-cpp JWT authentication
        find_package(jwt-cpp REQUIRED)
    else ()
        message("Using JWT from submodules...")
        if (NOT EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/third_party/jwt/CMakeLists.txt)
            message(SEND_ERROR "Unable to add a dependency locally!\nHave you tried running:  git submodule update --init --recursive")
        endif ()
        # HACK: workaround for a windows bug https://github.com/kazuho/picojson/issues/141
        set(JWT_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
        set(JWT_DISABLE_PICOJSON ON CACHE BOOL "" FORCE)
        add_subdirectory(third_party/jwt)
    endif ()
endif ()

# Add local nlohmann
if (BUILD_ALL OR BUILD_SERVER OR BUILD_GAMES)
    option(USE_EXTERNAL_NLOHMANN "Whether to use external nlohmann_json" OFF)
    if (USE_EXTERNAL_NLOHMANN)
        message("Using external Nlohmann...")
        # https://github.com/nlohmann/json/releases/tag/v3.9.0 for convenience macros
        find_package(nlohmann_json 3.9.0 REQUIRED)
    else ()
        message("Using Nlohmann from submodules...")
        if (NOT EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/third_party/json/CMakeLists.txt)
            message(SEND_ERROR "Unable to add a dependency locally!\nHave you tried running:  git submodule update --init --recursive")
        endif ()
        add_subdirectory(third_party/json)
    endif ()
endif ()

## Add local qtkeychain
#if (BUILD_ALL OR BUILD_CLIENT)
#    message("Using Qtkeychain from submodules...")
#    if (NOT EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/third_party/qtkeychain/CMakeLists.txt)
#        message(SEND_ERROR "Unable to add a dependency locally!\nHave you tried running:  git submodule update --init --recursive")
#    endif()
#    if (QT_MAJOR_VERSION EQUAL 6)
#        set(BUILD_WITH_QT6 ON)
#    endif ()
#    add_subdirectory(third_party/qtkeychain)
#endif()

if (BUILD_ALL OR BUILD_SERVER)
    add_subdirectory(server)
endif ()
if (BUILD_ALL OR BUILD_CLIENT)
    add_subdirectory(client)
endif ()
if (BUILD_ALL OR BUILD_GAMES)
    add_subdirectory(games)
endif ()

# Doxygen Documentation
find_package(Doxygen 1.9.1 OPTIONAL_COMPONENTS dot)
# Configure all options
include(cmake/DoxygenConfig.cmake)

if (DOXYGEN_FOUND)
    doxygen_add_docs(
        docs
        cavoke-dev-lib
        cavoke-dev-server-lib
        client
        server
        docs/mainpage.md
        docs/Introduction
        docs/Quick-Start
        docs/Deep-Dive
        COMMENT "Generate html docs"
    )
endif (DOXYGEN_FOUND)