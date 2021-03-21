target_precompile_headers(${PROJECT_NAME} PUBLIC include.h PUBLIC SQL.h PUBLIC database.h PUBLIC table.h PUBLIC column.h)

add_library(column column.cpp)
add_library(table table.cpp)
add_library(database database.cpp)
add_library(SQL SQL.cpp)
