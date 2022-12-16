#pragma once
#include <functional>
#include "common.h"

class Acceptor
{
private:
    EventLoop *loop;
    Socket *sock;
    Channel *channel;
    std::function<void(Socket*)> newConnectionCallback;
public:
    DISALLOW_COPY_AND_MOVE(Acceptor);
    explicit Acceptor(EventLoop *_loop);
    ~Acceptor();

    void acceptConnection();
    void setNewConnectionCallback(std::function<void(Socket*)>);
};