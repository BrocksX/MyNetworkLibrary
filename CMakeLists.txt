# cmake 最低版本需求
cmake_minimum_required(VERSION 3.13)

# 工程名称
project(myTinyServer)

# 添加搜索路径
include_directories(
    ${PROJECT_SOURCE_DIR}/src/base
    ${PROJECT_SOURCE_DIR}/src/mysql
    ${PROJECT_SOURCE_DIR}/src/net
    ${PROJECT_SOURCE_DIR}/src/redis
    ${PROJECT_SOURCE_DIR}/src/timer
    )


# 包含这些目录下的.cpp文件
aux_source_directory(${PROJECT_SOURCE_DIR}/src/base SRC_BASE)
aux_source_directory(${PROJECT_SOURCE_DIR}/src/mysql SRC_MYSQL)
aux_source_directory(${PROJECT_SOURCE_DIR}/src/net SRC_NET)
aux_source_directory(${PROJECT_SOURCE_DIR}/src/redis SRC_REDIS)
aux_source_directory(${PROJECT_SOURCE_DIR}/src/timer SRC_TIMER)

# 设置编译选项
set(CXX_FLAGS
    -g
    -Wall
    -std=c++17
    )

# 生成动态库 tiny_network
add_library(myTinyServer SHARED 
            ${SRC_BASE}
            ${SRC_MYSQL}
            ${SRC_NET}
            ${SRC_REDIS}
            ${SRC_TIMER}
            )

target_link_libraries(myTinyServer hiredis mysqlclient)


# 设置生成动态库的路径，放在根目录的lib文件夹下面
set(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/lib)
     
add_subdirectory(test)
