cmake_minimum_required(VERSION 3.10)
project(cavoke_server)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CAVOKE_H_DIR ../cavoke-dev-lib CACHE PATH "Directory with cavoke.h library")
include_directories(${CAVOKE_H_DIR})

aux_source_directory(model SRC_DIR)
aux_source_directory(controllers CTL_SRC)
aux_source_directory(filters FILTER_SRC)
aux_source_directory(plugins PLUGIN_SRC)
aux_source_directory(sql-models MODEL_SRC)

# C++ namespace with sql models drogon ORM
add_compile_definitions(cavoke_orm=cavoke)

add_executable(${PROJECT_NAME}
        main.cpp
        cavoke_base_exception.cpp
        ${FILTER_SRC}
        ${PLUGIN_SRC}
        ${MODEL_SRC}
        model/games/games_storage.cpp
        model/games/game.cpp
        controllers/games_controller.cpp
        controllers/health_controller.cpp
        model/games/games_storage_config.cpp
        model/logic/game_state_storage.cpp
        model/sessions/sessions_storage.cpp
        model/logic/game_logic_manager.cpp
        controllers/state_controller.cpp
        controllers/sessions_controller.cpp
        model/auth/authentication_manager.cpp
        model/sessions/game_session.cpp
        )
install(TARGETS ${PROJECT_NAME})

find_package(Boost 1.71 REQUIRED filesystem program_options)
include_directories(${Boost_INCLUDE_DIRS})
link_directories(${Boost_LIBRARY_DIRS})

find_package(Drogon 1.7.3)
if (NOT Drogon_FOUND)
    add_subdirectory(extern/drogon)
endif()

# https://github.com/nlohmann/json/releases/tag/v3.9.0 for convenience macros
find_package(nlohmann_json 3.9.0)
if (NOT nlohmann_json_FOUND)
    add_subdirectory(extern/json)
endif()

target_link_libraries(cavoke_server ${Boost_LIBRARIES} ${CMAKE_DL_LIBS})
target_link_libraries(cavoke_server drogon)
target_link_libraries(cavoke_server nlohmann_json)

drogon_create_views(${PROJECT_NAME} ${CMAKE_CURRENT_SOURCE_DIR}/views
        ${CMAKE_CURRENT_BINARY_DIR})
# use the following line to create views with namespaces.
# drogon_create_views(${PROJECT_NAME} ${CMAKE_CURRENT_SOURCE_DIR}/views
#                     ${CMAKE_CURRENT_BINARY_DIR} TRUE)

target_include_directories(${PROJECT_NAME}
        PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
# ##############################################################################
# uncomment the following line for dynamically loading views
# set_property(TARGET ${PROJECT_NAME} PROPERTY ENABLE_EXPORTS ON)

# ##############################################################################

add_subdirectory(test)