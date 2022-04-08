cmake_minimum_required(VERSION 3.10)
project(tictactoe)

set(CMAKE_CXX_STANDARD 17)

find_package(nlohmann_json 3.9.0 REQUIRED)

add_executable(tictactoe
        tictactoe/tictactoe.cpp
        cavoke.cpp)

add_executable(codenames
        codenames/server/src/codenames.cpp
        codenames/server/src/wordlists.cpp
        cavoke.cpp)

target_link_libraries(tictactoe nlohmann_json::nlohmann_json)
