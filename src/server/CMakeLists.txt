aux_source_directory(.  SRC1_LIST)
aux_source_directory(../util  SRC2_LIST)
aux_source_directory(../db  SRC3_LIST)
aux_source_directory(../public  SRC4_LIST)

# message("SRC_LIST:" ${SRC_LIST})
set(SRC_LIST )
list(APPEND SRC_LIST ${SRC1_LIST} ${SRC2_LIST} ${SRC3_LIST} ${SRC4_LIST})

add_executable(chatServer ${SRC_LIST})
target_link_libraries(chatServer muduo_net muduo_base mysqlclient pthread hiredis yaml-cpp spdlog)
