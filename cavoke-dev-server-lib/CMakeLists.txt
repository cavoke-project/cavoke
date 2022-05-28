cmake_minimum_required(VERSION 3.10)
project(cavoke_dev_server_lib)
set(CMAKE_CXX_STANDARD 17)

aux_source_directory(controllers CTL_SRC)

add_library(${PROJECT_NAME}
        cavoke.cpp
        controllers/logic_controller.cpp)

find_package(Boost 1.71 REQUIRED filesystem program_options)
include_directories(${Boost_INCLUDE_DIRS})
link_directories(${Boost_LIBRARY_DIRS})
target_link_libraries(${PROJECT_NAME} PRIVATE ${Boost_LIBRARIES} ${CMAKE_DL_LIBS})

# Link Drogon
if (USE_EXTERNAL_DROGON)
    target_link_libraries(${PROJECT_NAME} PRIVATE Drogon::Drogon)
else ()
    target_link_libraries(${PROJECT_NAME} PRIVATE drogon)
endif ()


if (USE_EXTERNAL_NLOHMANN)
    target_link_libraries(${PROJECT_NAME} PRIVATE nlohmann_json::nlohmann_json)
else ()
    target_link_libraries(${PROJECT_NAME} PRIVATE nlohmann_json)
endif ()
