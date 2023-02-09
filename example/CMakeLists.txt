# 编译源码生成目标
add_executable(echo_server echo_server.cpp)
target_link_libraries(echo_server myNetworkLibrary)

add_executable(echo_client echo_client.cpp)
target_link_libraries(echo_client myNetworkLibrary)

add_executable(echo_mul_clients echo_mul_clients.cpp)
target_link_libraries(echo_mul_clients myNetworkLibrary)

add_executable(chat_server chat_server.cpp)
target_link_libraries(chat_server myNetworkLibrary)
