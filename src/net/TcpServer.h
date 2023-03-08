#pragma once
#include "noncopyable.h"

#include <functional>
#include <unordered_map>
#include <vector>
#include <memory>
#include <cassert>

class EventLoop;
class Acceptor;
class Connection;
class Socket;
class ThreadPool;
/**
 * TcpServer 是一个可以支持多线程主从Reactor模式的服务器，通过注册回调函数实现业务逻辑
*/
class TcpServer
{
public:
    explicit TcpServer(EventLoop *loop, const char* ip, const uint16_t &port);
    ~TcpServer();

    DISALLOW_COPY_AND_MOVE(TcpServer);

    void newConnection(Socket *sock);
    void deleteConnection(std::shared_ptr<Connection>conn);
    void setMessageCallback(std::function<void(std::shared_ptr<Connection>)> fn);
    void setConnectionCallback(std::function<void(std::shared_ptr<Connection>)> fn);
    void start();

private:
    std::unique_ptr<EventLoop> mainReactor_;
    std::vector<EventLoop* > subReactors_;

    std::unique_ptr<Acceptor> acceptor_;
    std::unordered_map<int, std::shared_ptr<Connection>> connections_;

    std::unique_ptr<ThreadPool>threadPool_;
    std::function<void(std::shared_ptr<Connection>)> connectionCallback_;
    std::function<void(std::shared_ptr<Connection>)> messageCallback_;

    void createEventLoopThread();
    void deleteConnectionInLoop(std::shared_ptr<Connection>conn);
};