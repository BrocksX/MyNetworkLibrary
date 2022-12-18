#include "TcpServer.h"
#include <unistd.h>
#include <functional>
#include "Acceptor.h"
#include "Connection.h"
#include "EventLoop.h"
#include "Socket.h"
#include "ThreadPool.h"
#include "util.h"

TcpServer::TcpServer()
{
    mainReactor_ = std::make_unique<EventLoop>();
    acceptor_ = std::make_unique<Acceptor>(mainReactor_.get());

    std::function<void(Socket *)> cb = std::bind(&TcpServer::NewConnection, this, std::placeholders::_1);
    acceptor_->setNewConnectionCallback(cb);

    int size = static_cast<int>(std::thread::hardware_concurrency());
    threadPool_ = std::make_unique<ThreadPool>(size);
    for (size_t i = 0; i < size; ++i)
    {
        sub_reactors_.push_back(std::move(std::make_unique<EventLoop>()));
    }
}

TcpServer::~TcpServer() {}

void TcpServer::NewConnection(Socket *sock)
{
    errif(sock->getFd() == -1, "new connection error");
    uint64_t random = sock->getFd() % sub_reactors_.size();
    std::unique_ptr<Connection> conn = std::make_unique<Connection>(sub_reactors_[random].get(), sock);
    std::function<void(Socket *)> cb = std::bind(&TcpServer::DeleteConnection, this, std::placeholders::_1);
    conn->setDeleteConnectionCallback(cb);
    conn->setOnConnectCallback(on_connect_callback_);
    connections_[sock->getFd()] = std::move(conn);
}

void TcpServer::DeleteConnection(Socket *sock)
{
    int sockfd = sock->getFd();
    auto it = connections_.find(sockfd);
    assert(it != connections_.end());
    connections_.erase(sockfd);
}

void TcpServer::OnConnect(std::function<void(Connection *)> fn) { on_connect_callback_ = std::move(fn); }

void TcpServer::start()
{
    for (size_t i = 0; i < sub_reactors_.size(); ++i)
    {
        std::function<void()> sub_loop = std::bind(&EventLoop::loop, sub_reactors_[i].get());
        threadPool_->add(std::move(sub_loop));
    }
    mainReactor_->loop();
}