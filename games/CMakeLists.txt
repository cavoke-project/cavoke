cmake_minimum_required(VERSION 3.10)
project(tictactoe)


set(CAVOKE_H_DIR ../cavoke-dev-lib CACHE PATH "Directory with cavoke.h library")
include_directories(${CAVOKE_H_DIR})

add_executable(tictactoe
        tictactoe/tictactoe.cpp
        ${CAVOKE_H_DIR}/cavoke.cpp)

option(USE_EXTERNAL_NLOHMANN "Whether to use external nlohmann_json" OFF)
if (USE_EXTERNAL_NLOHMANN)
    message("Using external Nlohmann...")
    # https://github.com/nlohmann/json/releases/tag/v3.9.0 for convenience macros
    find_package(nlohmann_json 3.9.0 REQUIRED)
    target_link_libraries(tictactoe nlohmann_json::nlohmann_json)
else()
    message("Using Nlohmann from submodules...")
    # FIXME: will crash, if building games without server!
    #  as discussed, games should be moved into their own repos
    target_link_libraries(tictactoe nlohmann_json)
endif()