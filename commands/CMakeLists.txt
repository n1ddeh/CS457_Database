target_precompile_headers(${PROJECT_NAME} PRIVATE cmd.h)

add_library(cmd cmd.cpp)
add_library(create create.cpp)
add_library(drop drop.cpp)
add_library(use use.cpp)
add_library(alter alter.cpp)