SET(DESTINATION ${PROJECT_SOURCE_DIR})

add_library(util SHARED util.cpp)
add_library(sqlutil SHARED db.cpp)

target_link_libraries(sqlutil mysqlclient)
