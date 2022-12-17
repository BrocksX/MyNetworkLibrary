#pragma once
#include <functional>
#include "common.h"

class Acceptor
{
private:
    EventLoop *loop_;
    Socket *sock_;
    Channel *channel_;
    std::function<void(Socket*)> newConnectionCallback_;
public:
    DISALLOW_COPY_AND_MOVE(Acceptor);
    explicit Acceptor(EventLoop *_loop);
    ~Acceptor();

    void acceptConnection() const;
    void setNewConnectionCallback(std::function<void(Socket*)> const & _cb);
};