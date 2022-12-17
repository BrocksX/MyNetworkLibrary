# cmake 最低版本需求
cmake_minimum_required(VERSION 3.13)

# 工程名称
project(myTinyServer)

# 添加搜索路径
include_directories(
    ${PROJECT_SOURCE_DIR}/src/base
    ${PROJECT_SOURCE_DIR}/src/net
    )


# 包含这些目录下的.cpp文件
aux_source_directory(${PROJECT_SOURCE_DIR}/src/base SRC_BASE)
aux_source_directory(${PROJECT_SOURCE_DIR}/src/net SRC_NET)


# 设置编译选项
set(CXX_FLAGS
    -g
    -Wall
    -std=c++11
    )

# 生成动态库 tiny_network
add_library(myTinyServer SHARED 
            ${SRC_BASE}
            ${SRC_NET}
            )

# 设置生成动态库的路径，放在根目录的lib文件夹下面
set(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/lib)
     
add_subdirectory(test)
