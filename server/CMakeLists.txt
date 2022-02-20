cmake_minimum_required(VERSION 3.10)
project(cavoke_server)

set(CMAKE_CXX_STANDARD 17)

find_package(Drogon REQUIRED)

find_package(Boost 1.71 REQUIRED filesystem program_options)
include_directories(${Boost_INCLUDE_DIRS})
link_directories(${Boost_LIBRARY_DIRS})

# https://github.com/nlohmann/json/releases/tag/v3.9.0 for convenience macros
find_package(nlohmann_json 3.9.0 REQUIRED)

add_executable(cavoke_server
        model/games_storage.cpp
        model/game.cpp
        controllers/games_controller.cpp
        main.cpp
        controllers/health_controller.cpp
        controllers/health_controller.h
        model/GamesStorageConfig.cpp
        model/GamesStorageConfig.h
        utils.h
        model/game_state_storage.cpp model/participation_storage.cpp model/game_logic_manager.cpp model/mock_tictactoe/tictactoe.cpp controllers/state_controller.cpp
        )
target_link_libraries(cavoke_server Drogon::Drogon)
target_link_libraries(cavoke_server ${Boost_LIBRARIES} ${CMAKE_DL_LIBS})
target_link_libraries(cavoke_server nlohmann_json::nlohmann_json)
