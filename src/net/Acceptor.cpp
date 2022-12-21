#include "Acceptor.h"
#include "Socket.h"
#include "Channel.h"
#include <cstdio>

Acceptor::Acceptor(EventLoop *loop, const char* ip, uint16_t port)
{
    socket_ = std::make_unique<Socket>();
    InetAddress *addr = new InetAddress(ip,port);
    socket_->bind(addr);
    //sock_->setnonblocking();
    socket_->listen();
    channel_ = std::make_unique<Channel>(loop,socket_->getFd());
    std::function<void()> cb = std::bind(&Acceptor::acceptConnection, this);
    channel_->setReadCallback(cb);
    channel_->enableRead();
    delete addr;
}
Acceptor::~Acceptor() {}
void Acceptor::acceptConnection() const
{
    InetAddress *clnt_addr = new InetAddress();
    Socket *clnt_sock = new Socket(socket_->accept(clnt_addr));
    printf("new client fd %d! IP: %s Port: %d\n", clnt_sock->getFd(), inet_ntoa(clnt_addr->getAddr().sin_addr), ntohs(clnt_addr->getAddr().sin_port));
    clnt_sock->setNonBlocking();
    newConnectionCallback_(clnt_sock);
    delete clnt_addr;
}
void Acceptor::setNewConnectionCallback(std::function<void(Socket*)> const & _cb)
{
    newConnectionCallback_ = _cb;
}