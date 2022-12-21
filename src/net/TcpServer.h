#pragma once
#include "common.h"

#include <functional>
#include <unordered_map>
#include <vector>

class TcpServer
{
private:
    std::unique_ptr<EventLoop> mainReactor_;
    std::vector<std::unique_ptr<EventLoop>> sub_reactors_;

    std::unique_ptr<Acceptor> acceptor_;
    std::unordered_map<int, std::unique_ptr<Connection>> connections_;

    std::unique_ptr<ThreadPool>threadPool_;
    std::function<void(Connection *)> onConnectCallback_;
    std::function<void(Connection *)> onRecvCallback_;

public:
    explicit TcpServer(const char* ip, uint16_t port);
    ~TcpServer();

    DISALLOW_COPY_AND_MOVE(TcpServer);

    void newConnection(Socket *sock);
    void deleteConnection(Socket *sock);
    void setOnRecvCallback(std::function<void(Connection *)> fn);
    void setOnConnectCallback(std::function<void(Connection *)> fn);
    void start();
};