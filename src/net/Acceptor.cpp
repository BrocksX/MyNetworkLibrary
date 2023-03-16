#include "Acceptor.h"
#include "Socket.h"
#include "Channel.h"
#include <cstdio>


Acceptor::Acceptor(EventLoop *loop, const char* ip, const uint16_t &port) : socket_(std::make_unique<Socket>())
{
    InetAddress addr = InetAddress(ip, port);
    socket_->bind(&addr);
    socket_->listen();
    channel_ = std::make_unique<Channel>(loop, socket_->getFd());
    channel_->setReadCallback(std::bind(&Acceptor::acceptConnection, this));
    channel_->enableReading();
}

Acceptor::~Acceptor() {}

void Acceptor::acceptConnection() const
{
    InetAddress clnt_addr;
    Socket *clnt_sock = new Socket(socket_->accept(&clnt_addr));
    printf("new connection, IP: %s Port: %d\n", inet_ntoa(clnt_addr.getAddr().sin_addr), ntohs(clnt_addr.getAddr().sin_port));
    clnt_sock->setNonBlocking();
    newConnectionCallback_(clnt_sock);
}
void Acceptor::setNewConnectionCallback(std::function<void(Socket*)> const & _cb)
{
    newConnectionCallback_ = _cb;
}