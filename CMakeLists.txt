cmake_minimum_required(VERSION 3.10)
project(cavoke)

option(BUILD_SERVER "Enable building server" ON)
option(BUILD_CLIENT "Enable building client" ON)

if(BUILD_SERVER)
    add_subdirectory(server)
endif()
if(BUILD_CLIENT)
    add_subdirectory(client)
endif()