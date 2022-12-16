#pragma once
#include <map>
#include <vector>
#include <functional>
#include "common.h"
class Server
{
private:
    EventLoop *mainReactor;
    Acceptor *acceptor;
    std::map<int,Connection*> connections;
    std::vector<EventLoop*> subReactor;
    ThreadPool *thpool;
    std::function<void(Connection *)> on_connect_callback_;
public:
    DISALLOW_COPY_AND_MOVE(Server);
    Server(EventLoop*);
    ~Server();

    void handleReadEvent(int);
    void newConnection(Socket *sock);
    void deleteConnection(Socket *sock);
    void OnConnect(std::function<void(Connection *)> fn);
};
