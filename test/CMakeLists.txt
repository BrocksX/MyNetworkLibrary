# 编译源码生成目标
add_executable(echo_server echo_server.cpp)
target_link_libraries(echo_server myTinyServer)

add_executable(echo_client echo_client.cpp)
target_link_libraries(echo_client myTinyServer)

add_executable(echo_mul_clients echo_mul_clients.cpp)
target_link_libraries(echo_mul_clients myTinyServer)

add_executable(chat_server chat_server.cpp)
target_link_libraries(chat_server myTinyServer)

add_executable(TEST test.cpp)
target_link_libraries(TEST myTinyServer)

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/example)

