cmake_minimum_required(VERSION 3.10)
project(games)

set(CMAKE_CXX_STANDARD 17)

set(CAVOKE_H_DIR ../cavoke-dev-lib CACHE PATH "Directory with cavoke.h library" FORCE)
set(CAVOKE_DEV_SERVER_H_DIR ../cavoke-dev-server-lib CACHE PATH "Directory with game dev server library" FORCE)
include_directories(${CAVOKE_H_DIR})
include_directories(${CAVOKE_DEV_SERVER_H_DIR})

add_executable(tictactoe
        tictactoe/tictactoe.cpp
        ${CAVOKE_H_DIR}/cavoke.cpp)

add_executable(codenames
        codenames/server/src/codenames.cpp
        codenames/server/src/wordlists.cpp
        ${CAVOKE_H_DIR}/cavoke.cpp codenames/server/src/model.cpp)

add_executable(tictactoe_server
        tictactoe/tictactoe.cpp
        ${CAVOKE_DEV_SERVER_H_DIR}/cavoke.cpp
        ${CAVOKE_DEV_SERVER_H_DIR}/controllers/logic_controller.cpp)

add_executable(codenames_server
        codenames/server/src/codenames.cpp
        codenames/server/src/wordlists.cpp
        codenames/server/src/model.cpp
        ${CAVOKE_DEV_SERVER_H_DIR}/cavoke.cpp
        ${CAVOKE_DEV_SERVER_H_DIR}/controllers/logic_controller.cpp)

if (USE_EXTERNAL_NLOHMANN)
    target_link_libraries(tictactoe PRIVATE nlohmann_json::nlohmann_json)
    target_link_libraries(tictactoe_server PRIVATE nlohmann_json::nlohmann_json)
    target_link_libraries(codenames PRIVATE nlohmann_json::nlohmann_json)
    target_link_libraries(codenames_server PRIVATE nlohmann_json::nlohmann_json)
else ()
    target_link_libraries(tictactoe PRIVATE nlohmann_json)
    target_link_libraries(tictactoe_server PRIVATE nlohmann_json)
    target_link_libraries(codenames PRIVATE nlohmann_json)
    target_link_libraries(codenames_server PRIVATE nlohmann_json)
endif ()

# Link Drogon
if (USE_EXTERNAL_DROGON)
    target_link_libraries(tictactoe_server PRIVATE Drogon::Drogon)
    target_link_libraries(codenames_server PRIVATE Drogon::Drogon)
else ()
    target_link_libraries(tictactoe_server PRIVATE drogon)
    target_link_libraries(codenames_server PRIVATE drogon)
endif ()

# Link Boost
find_package(Boost 1.71 REQUIRED filesystem program_options)
include_directories(${Boost_INCLUDE_DIRS})
link_directories(${Boost_LIBRARY_DIRS})

target_link_libraries(tictactoe_server PRIVATE ${Boost_LIBRARIES} ${CMAKE_DL_LIBS})
target_link_libraries(codenames_server PRIVATE ${Boost_LIBRARIES} ${CMAKE_DL_LIBS})
