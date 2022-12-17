#include "TcpServer.h"

#include <unistd.h>

#include <functional>

#include "Acceptor.h"
#include "Connection.h"
#include "EventLoop.h"
#include "Socket.h"
#include "ThreadPool.h"
#include "util.h"

Server::Server(EventLoop *loop) : mainReactor_(loop), acceptor_(nullptr), thread_pool_(nullptr)
{
    acceptor_ = new Acceptor(mainReactor_);
    std::function<void(Socket *)> cb = std::bind(&Server::NewConnection, this, std::placeholders::_1);
    acceptor_->setNewConnectionCallback(cb);

    int size = static_cast<int>(std::thread::hardware_concurrency());
    thread_pool_ = new ThreadPool(size);
    for (int i = 0; i < size; ++i)
    {
        subReactors_.push_back(new EventLoop());
    }

    for (int i = 0; i < size; ++i)
    {
        std::function<void()> sub_loop = std::bind(&EventLoop::loop, subReactors_[i]);
        thread_pool_->add(std::move(sub_loop));
    }
}

Server::~Server()
{
    delete acceptor_;
    delete thread_pool_;
}

void Server::NewConnection(Socket *sock)
{
    errif(sock->getFd() == -1, "new connection error");
    uint64_t random = sock->getFd() % subReactors_.size();
    Connection *conn = new Connection(subReactors_[random], sock);
    std::function<void(Socket *)> cb = std::bind(&Server::DeleteConnection, this, std::placeholders::_1);
    conn->setDeleteConnectionCallback(cb);
    conn->setOnConnectCallback(onConnectCallback_);
    connections_[sock->getFd()] = conn;
}

void Server::DeleteConnection(Socket *sock)
{
    int sockfd = sock->getFd();
    auto it = connections_.find(sockfd);
    if (it != connections_.end())
    {
        Connection *conn = connections_[sockfd];
        connections_.erase(sockfd);
        delete conn;
        conn = nullptr;
    }
}

void Server::OnConnect(std::function<void(Connection *)> fn) { onConnectCallback_ = std::move(fn); }