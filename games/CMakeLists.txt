cmake_minimum_required(VERSION 3.10)
project(tictactoe)

find_package(nlohmann_json 3.9.0 REQUIRED)

add_executable(tictactoe
        tictactoe/tictactoe.cpp
        ../cavoke-dev-lib/cavoke.cpp)

target_link_libraries(tictactoe nlohmann_json::nlohmann_json)
