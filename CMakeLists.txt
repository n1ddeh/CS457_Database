cmake_minimum_required(VERSION 3.19.4)

set (CMAKE_CXX_STANDARD 17)

project(SCHEMA)

add_executable(${PROJECT_NAME} main.cpp)

add_subdirectory(database)

target_include_directories(${PROJECT_NAME} PUBLIC database)

target_link_directories(${PROJECT_NAME} PRIVATE database)

target_link_libraries(${PROJECT_NAME} SQL database table column)

include(CheckCXXCompilerFlag)
CHECK_CXX_COMPILER_FLAG("-std=c++17" COMPILER_SUPPORTS_CXX17)
CHECK_CXX_COMPILER_FLAG("-std=c++0x" COMPILER_SUPPORTS_CXX0X)
if(COMPILER_SUPPORTS_CXX17)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17")
elseif(COMPILER_SUPPORTS_CXX0X)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x")
else()
        message(STATUS "The compiler ${CMAKE_CXX_COMPILER} has no C++17 support. Please use a different C++ compiler.")
endif()