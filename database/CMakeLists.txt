target_precompile_headers(${PROJECT_NAME} PUBLIC SQL.h database.h table.h column.h include.h)

add_library(column column.cpp)
add_library(table table.cpp)
add_library(database database.cpp)
add_library(SQL SQL.cpp)


