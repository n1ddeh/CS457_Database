target_precompile_headers(${PROJECT_NAME} PRIVATE SQL.h database.h table.h column.h)

add_library(SQL SQL.cpp)
add_library(database database.cpp)
add_library(table table.cpp)
add_library(column column.cpp)