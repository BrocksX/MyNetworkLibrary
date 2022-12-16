#include "Acceptor.h"
#include "Socket.h"
#include "Channel.h"
#include <cstdio>

Acceptor::Acceptor(EventLoop *_loop) : loop(_loop)
{
    sock = new Socket();
    InetAddress *addr = new InetAddress("127.0.0.1",8888);
    sock->bind(addr);
    //sock->setnonblocking();
    sock->listen();
    channel = new Channel(loop, sock->getFd());
    std::function<void()> cb = std::bind(&Acceptor::acceptConnection, this);
    channel->setReadCallback(cb);
    channel->enableReading();
    delete addr;
}
Acceptor::~Acceptor()
{
    delete sock;
    delete channel;
}
void Acceptor::acceptConnection()
{
    InetAddress *clnt_addr = new InetAddress();
    Socket *clnt_sock = new Socket(sock->accept(clnt_addr));
    printf("new client fd %d! IP: %s Port: %d\n", clnt_sock->getFd(), inet_ntoa(clnt_addr->getAddr().sin_addr), ntohs(clnt_addr->getAddr().sin_port));
    clnt_sock->setnonblocking();
    newConnectionCallback(clnt_sock);
    delete clnt_addr;
}
void Acceptor::setNewConnectionCallback(std::function<void(Socket*)> _cb)
{
    newConnectionCallback = _cb;
}