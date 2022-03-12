cmake_minimum_required(VERSION 3.10)
project(cavoke)

option(BUILD_ALL "Build all subprojects (usually disabled for ci)" ON)

option(BUILD_SERVER "Enable building server" OFF)
option(BUILD_CLIENT "Enable building client" OFF)
option(BUILD_GAMES "Enable building games" OFF)

if(BUILD_ALL OR BUILD_SERVER)
    add_subdirectory(server)
endif()
if(BUILD_ALL OR BUILD_CLIENT)
    add_subdirectory(client)
endif()
if(BUILD_ALL OR BUILD_GAMES)
    add_subdirectory(games)
endif()
