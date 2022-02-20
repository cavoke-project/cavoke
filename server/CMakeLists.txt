cmake_minimum_required(VERSION 3.10)
project(cavoke_server)

set(CMAKE_CXX_STANDARD 17)

find_package(Drogon REQUIRED)

find_package(Boost 1.71 REQUIRED filesystem)
include_directories(${Boost_INCLUDE_DIRS})
link_directories(${Boost_LIBRARY_DIRS})

add_executable(cavoke_server model/games_storage.cpp model/game.cpp controllers/games_controller.cpp main.cpp controllers/health_controller.cpp controllers/health_controller.h model/game_state_storage.cpp model/game_state_storage.h model/participation_storage.cpp model/participation_storage.h model/game_logic_manager.cpp model/game_logic_manager.h model/mock_tictactoe/tictactoe.cpp model/mock_tictactoe/tictactoe.h)
target_link_libraries(cavoke_server Drogon::Drogon)
target_link_libraries(cavoke_server ${Boost_LIBRARIES} ${CMAKE_DL_LIBS})
