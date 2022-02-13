cmake_minimum_required(VERSION 3.21)
project(cavoke_server)

set(CMAKE_CXX_STANDARD 17)

find_package(Drogon REQUIRED)

find_package(Boost 1.71 REQUIRED filesystem)
include_directories(${Boost_INCLUDE_DIRS})
link_directories(${Boost_LIBRARY_DIRS})

add_executable(cavoke_server model/games_storage.cpp controllers/games_controller.cpp main.cpp)
target_link_libraries(cavoke_server Drogon::Drogon)
target_link_libraries(cavoke_server ${Boost_LIBRARIES} ${CMAKE_DL_LIBS})
