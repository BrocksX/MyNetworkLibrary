#pragma once
#include <functional>
#include "nocopyable.h"
#include "EventLoop.h"
#include <string>
#include <memory>

class Socket;
class Channel;
class Acceptor;

class Acceptor
{
public:
    DISALLOW_COPY_AND_MOVE(Acceptor);
    explicit Acceptor(EventLoop *loop, const char* ip, const uint16_t &port);
    ~Acceptor();

    void acceptConnection() const;
    void setNewConnectionCallback(std::function<void(Socket*)> const & _cb);

private:
    std::unique_ptr<Socket> socket_;
    std::unique_ptr<Channel> channel_;
    std::function<void(Socket*)> newConnectionCallback_;
    
};