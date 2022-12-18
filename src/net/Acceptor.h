#pragma once
#include <functional>
#include "common.h"

class Acceptor
{
private:
    std::unique_ptr<Socket> socket_;
    std::unique_ptr<Channel> channel_;
    std::function<void(Socket*)> newConnectionCallback_;
public:
    DISALLOW_COPY_AND_MOVE(Acceptor);
    explicit Acceptor(EventLoop *loop);
    ~Acceptor();

    void acceptConnection() const;
    void setNewConnectionCallback(std::function<void(Socket*)> const & _cb);
};