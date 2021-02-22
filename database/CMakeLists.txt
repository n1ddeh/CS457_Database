target_precompile_headers(${PROJECT_NAME} PUBLIC column.h table.h database.h SQL.h)

add_library(column column.cpp)
add_library(table table.cpp)
add_library(database database.cpp)
add_library(SQL SQL.cpp)


