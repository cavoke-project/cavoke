cmake_minimum_required(VERSION 3.10)
project(cavoke_server_test CXX)

add_executable(${PROJECT_NAME} test_main.cc)

# ##############################################################################
# If you include the drogon source code locally in your project, use this method
# to add drogon 
# target_link_libraries(${PROJECT_NAME}_test PRIVATE drogon)
#
# and comment out the following lines
if (USE_EXTERNAL_DROGON)
    target_link_libraries(${PROJECT_NAME} PRIVATE Drogon::Drogon)
else ()
    target_link_libraries(${PROJECT_NAME} PRIVATE drogon)
endif ()

ParseAndAddDrogonTests(${PROJECT_NAME})
