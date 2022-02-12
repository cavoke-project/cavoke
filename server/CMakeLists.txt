cmake_minimum_required(VERSION 3.21)
project(cavoke_server)

set(CMAKE_CXX_STANDARD 17)

find_package(Drogon REQUIRED)

find_package(Boost 1.71 REQUIRED filesystem)
include_directories(${Boost_INCLUDE_DIRS})
link_directories(${Boost_LIBRARY_DIRS})

find_package(nlohmann_json 3.2.0 REQUIRED)

add_executable(cavoke_server model/games_storage.cpp)
target_link_libraries(cavoke_server Drogon::Drogon)
target_link_libraries(cavoke_server ${Boost_LIBRARIES} ${CMAKE_DL_LIBS})
target_link_libraries(cavoke_server nlohmann_json::nlohmann_json)
