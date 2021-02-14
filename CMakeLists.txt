cmake_minimum_required(VERSION 3.19.4)

project(SCHEMA)

add_executable(${PROJECT_NAME} main.cpp)

add_subdirectory(database)
add_subdirectory(commands)

target_include_directories(${PROJECT_NAME} PUBLIC database PUBLIC commands)

target_link_directories(${PROJECT_NAME} PRIVATE database PRIVATE commands)

target_link_libraries(${PROJECT_NAME} SQL database table column cmd create drop use alter)