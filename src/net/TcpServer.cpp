#include "TcpServer.h"
#include <unistd.h>
#include <functional>
#include "Acceptor.h"
#include "Connection.h"
#include "EventLoop.h"
#include "Socket.h"
#include "ThreadPool.h"

TcpServer::TcpServer(EventLoop *loop, const char* ip, const uint16_t &port)
{
    mainReactor_ = std::unique_ptr<EventLoop>(loop);
    acceptor_ = std::make_unique<Acceptor>(mainReactor_.get(), ip, port);

    std::function<void(Socket *)> cb = std::bind(&TcpServer::newConnection, this, std::placeholders::_1);
    acceptor_->setNewConnectionCallback(cb);

    int size = static_cast<int>(std::thread::hardware_concurrency());
    threadPool_ = std::make_unique<ThreadPool>(size);
    for (size_t i = 0; i < size; ++i)
    {
        sub_reactors_.push_back(std::move(std::make_unique<EventLoop>()));
    }
}

TcpServer::~TcpServer() {}

void TcpServer::newConnection(Socket *sock)
{
    if(sock->getFd() == -1)
        throw std::runtime_error("new connection error");
    uint64_t random = sock->getFd() % sub_reactors_.size();
    std::unique_ptr<Connection> conn = std::make_unique<Connection>(sub_reactors_[random].get(), sock);
    std::function<void(Socket *)> cb = std::bind(&TcpServer::deleteConnection, this, std::placeholders::_1);
    conn->setDeleteConnectionCallback(cb);
    conn->setMessageCallback(messageCallback_);
    connections_[sock->getFd()] = std::move(conn);

    if(connectionCallback_)
        connectionCallback_(connections_[sock->getFd()].get());
}

void TcpServer::deleteConnection(Socket *sock)
{
    int sockfd = sock->getFd();
    auto it = connections_.find(sockfd);
    assert(it != connections_.end());
    connections_.erase(sockfd);
}

void TcpServer::setMessageCallback(std::function<void(Connection *)> fn) { messageCallback_ = std::move(fn); }

void TcpServer::setConnectionCallback(std::function<void(Connection *)> fn) { connectionCallback_ = std::move(fn); }

void TcpServer::start()
{
    for (size_t i = 0; i < sub_reactors_.size(); ++i)
    {
        std::function<void()> sub_loop = std::bind(&EventLoop::loop, sub_reactors_[i].get());
        threadPool_->add(std::move(sub_loop));
    }
    mainReactor_->loop();
}

