add_executable(sql_test sql_test.cpp)
add_test(NAME SqlTest COMMAND sql_test)

target_link_libraries(sql_test util)