#include "Server.h"
#include "Socket.h"
#include <functional>
#include <unistd.h>
#include "Acceptor.h"
#include "Connection.h"
#include "ThreadPool.h"
#include "EventLoop.h"

Server::Server(EventLoop *_loop):mainReactor(_loop), acceptor(nullptr)
{
    acceptor = new Acceptor(mainReactor);
    std::function<void(Socket*)> cb = std::bind(&Server::newConnection, this, std::placeholders::_1);
    acceptor->setNewConnectionCallback(cb);

    int size = std::thread::hardware_concurrency();
    thpool = new ThreadPool(size);
    for(int i = 0; i < size; ++i)
        subReactor.emplace_back(new EventLoop());
    for(int i = 0; i < size; ++i)
    {
        std::function<void()> sub_loop = std::bind(&EventLoop::loop, subReactor[i]);
        thpool->add(sub_loop);
    }
}
Server::~Server()
{
    delete acceptor;
    delete thpool;
}
void Server::newConnection(Socket* sock)
{
    if(sock->getFd() != -1)
    {
        int random = sock->getFd() % subReactor.size();
        Connection *conn = new Connection(subReactor[random], sock);
        std::function<void(Socket *)> cb = std::bind(&Server::deleteConnection, this, std::placeholders::_1);
        conn->SetDeleteConnectionCallback(cb);
        conn->SetOnConnectCallback(on_connect_callback_);
        connections[sock->getFd()] = conn;
    }
}
void Server::deleteConnection(Socket *sock) {
  int sockfd = sock->getFd();
  auto it = connections.find(sockfd);
  if (it != connections.end()) {
    Connection *conn = connections[sockfd];
    connections.erase(sockfd);
    delete conn;
    conn = nullptr;
  }
}

void Server::OnConnect(std::function<void(Connection *)> fn) 
{ 
    on_connect_callback_ = std::move(fn); 
}
