# 编译源码生成目标
add_executable(server server.cpp)
target_link_libraries(server myTinyServer)

add_executable(thread ThreadPoolTest.cpp)
target_link_libraries(thread myTinyServer)

add_executable(client client.cpp)
target_link_libraries(client myTinyServer)

add_executable(test test.cpp)
target_link_libraries(test myTinyServer)

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/example)

