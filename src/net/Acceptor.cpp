#include "Acceptor.h"
#include "Socket.h"
#include "Channel.h"
#include <cstdio>

Acceptor::Acceptor(EventLoop *_loop) : loop_(_loop)
{
    sock_ = new Socket();
    InetAddress *addr = new InetAddress("127.0.0.1",8888);
    sock_->bind(addr);
    //sock_->setnonblocking();
    sock_->listen();
    channel_ = new Channel(loop_, sock_->getFd());
    std::function<void()> cb = std::bind(&Acceptor::acceptConnection, this);
    channel_->setReadCallback(cb);
    channel_->enableRead();
    delete addr;
}
Acceptor::~Acceptor()
{
    delete sock_;
    delete channel_;
}
void Acceptor::acceptConnection() const
{
    InetAddress *clnt_addr = new InetAddress();
    Socket *clnt_sock = new Socket(sock_->accept(clnt_addr));
    printf("new client fd %d! IP: %s Port: %d\n", clnt_sock->getFd(), inet_ntoa(clnt_addr->getAddr().sin_addr), ntohs(clnt_addr->getAddr().sin_port));
    clnt_sock->setNonBlocking();
    newConnectionCallback_(clnt_sock);
    delete clnt_addr;
}
void Acceptor::setNewConnectionCallback(std::function<void(Socket*)> const & _cb)
{
    newConnectionCallback_ = _cb;
}