aux_source_directory(../util SRC1_LIST)
aux_source_directory(../db SRC2_LIST)
aux_source_directory(../public SRC3_LIST)

add_executable(sql_test sql_test.cpp ${SRC1_LIST} ${SRC2_LIST} ${SRC3_LIST})
add_test(NAME SqlTest COMMAND sql_test)

target_link_libraries(sql_test mysqlclient hiredis yaml-cpp pthread)