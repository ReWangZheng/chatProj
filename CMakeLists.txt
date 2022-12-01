cmake_minimum_required(VERSION 3.0) #
project(main)
enable_testing()
# 配置编译选项
set(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} -g)
# 配置头文件搜索路径
# include_directories()
# 配置库文件搜索路径
# 设置需要编译的源文件列表
set(SRC_LIST ./server.cpp)
# 设置编译出的二进制文件的输出目录 
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)
set(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/lib)
# 把.指定路径下的所有源文件的名字放入变量名 SRC_LIST
# aux_source_directory(. SRC_LIST)
include_directories(${PROJECT_SOURCE_DIR}/include)
include_directories(${PROJECT_SOURCE_DIR}/include/server)
include_directories(${PROJECT_SOURCE_DIR}/include/server/db)
include_directories(${PROJECT_SOURCE_DIR}/thirdparty)



# 生成文件 ?
# 表示server这个可执行server，由SRC_LIST变量所定义的源文件编译而来
# bia
add_subdirectory(${PROJECT_SOURCE_DIR}/src)
