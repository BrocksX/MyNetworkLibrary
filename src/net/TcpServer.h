#pragma once
#include "common.h"

#include <functional>
#include <map>
#include <vector>
class Server
{
private:
    EventLoop *mainReactor_;
    Acceptor *acceptor_;
    std::map<int, Connection *> connections_;
    std::vector<EventLoop *> subReactors_;
    ThreadPool *thread_pool_;
    std::function<void(Connection *)> onConnectCallback_;

public:
    explicit Server(EventLoop *loop);
    ~Server();

    DISALLOW_COPY_AND_MOVE(Server);

    void NewConnection(Socket *sock);
    void DeleteConnection(Socket *sock);
    void OnConnect(std::function<void(Connection *)> fn);
};