cmake_minimum_required(VERSION 3.10)
project(cavoke_game_container_server)
set(CMAKE_CXX_STANDARD 17)

add_executable(${PROJECT_NAME}
        main.cpp)

find_package(Boost 1.71 REQUIRED filesystem program_options)
include_directories(${Boost_INCLUDE_DIRS})
link_directories(${Boost_LIBRARY_DIRS})

# Link Drogon
if (USE_EXTERNAL_DROGON)
    target_link_libraries(${PROJECT_NAME} PRIVATE Drogon::Drogon)
else ()
    target_link_libraries(${PROJECT_NAME} PRIVATE drogon)
endif ()
