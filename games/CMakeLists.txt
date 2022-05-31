cmake_minimum_required(VERSION 3.10)
project(tictactoe)

set(CMAKE_CXX_STANDARD 17)

set(CAVOKE_H_DIR ../cavoke-dev-lib CACHE PATH "Directory with cavoke.h library")
include_directories(${CAVOKE_H_DIR})

add_executable(tictactoe
        tictactoe/tictactoe.cpp
        ${CAVOKE_H_DIR}/cavoke.cpp)

add_executable(codenames
        codenames/server/src/codenames.cpp
        codenames/server/src/wordlists.cpp
        ${CAVOKE_H_DIR}/cavoke.cpp codenames/server/src/model.cpp)

add_executable(tictactoe_server
        tictactoe/tictactoe.cpp)

add_executable(codenames_server
        codenames/server/src/codenames.cpp
        codenames/server/src/wordlists.cpp
        codenames/server/src/model.cpp)

target_link_libraries(tictactoe_server PRIVATE cavoke_dev_server_lib)
target_link_libraries(codenames_server PRIVATE cavoke_dev_server_lib)

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
