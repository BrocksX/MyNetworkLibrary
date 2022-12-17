# 编译源码生成目标
add_executable(server server.cpp)
target_link_libraries(server myTinyServer)

add_executable(client client.cpp)
target_link_libraries(client myTinyServer)

add_executable(mul_clients mul_clients.cpp)
target_link_libraries(mul_clients myTinyServer)

add_executable(timertest timertest.cpp)
target_link_libraries(timertest myTinyServer)

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/example)

