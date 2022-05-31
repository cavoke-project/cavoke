cmake_minimum_required(VERSION 3.10)
project(cavoke_server)
set(CMAKE_CXX_STANDARD 17)

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
        controllers/users_controller.cpp
        model/sessions/game_session.cpp
        model/statistics/statistics_manager.cpp
        controllers/statistics_controller.cpp
        model/rooms/rooms_storage.cpp
        controllers/rooms_controller.cpp
        controllers/gamesubmissions_controller.cpp
        )
install(TARGETS ${PROJECT_NAME})

find_package(Boost 1.71 REQUIRED filesystem program_options)
include_directories(${Boost_INCLUDE_DIRS})
link_directories(${Boost_LIBRARY_DIRS})

# Link Drogon
if (USE_EXTERNAL_DROGON)
    target_link_libraries(${PROJECT_NAME} PRIVATE Drogon::Drogon)
else ()
    target_link_libraries(${PROJECT_NAME} PRIVATE drogon)
endif ()

# Link nlohmann
if (USE_EXTERNAL_NLOHMANN)
    target_link_libraries(${PROJECT_NAME} PRIVATE nlohmann_json::nlohmann_json)
else ()
    target_link_libraries(${PROJECT_NAME} PRIVATE nlohmann_json)
endif ()

# Link jwt
if (USE_EXTERNAL_NLOHMANN)
    target_link_libraries(${PROJECT_NAME} PRIVATE jwt-cpp::jwt-cpp)
else ()
    target_link_libraries(${PROJECT_NAME} PRIVATE jwt-cpp)
endif ()


target_link_libraries(cavoke_server PRIVATE ${Boost_LIBRARIES} ${CMAKE_DL_LIBS})

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
