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
}

TcpServer::~TcpServer() {}

void TcpServer::newConnection(Socket *sock)
{
    if(sock->getFd() == -1)
        throw std::runtime_error("new connection error");
    uint64_t random = sock->getFd() % subReactors_.size();
    std::shared_ptr<Connection> conn = std::make_shared<Connection>(subReactors_[random], sock);
    conn->setDeleteConnectionCallback(std::bind(&TcpServer::deleteConnection, this, std::placeholders::_1));
    conn->setMessageCallback(messageCallback_);
    connections_[sock->getFd()] = conn;
    if(connectionCallback_)
        connectionCallback_(connections_[sock->getFd()]);

    subReactors_[random]->queueInLoop(std::bind(&Connection::connectEstablished, conn));   
}

void TcpServer::deleteConnection(std::shared_ptr<Connection> conn)
{
    mainReactor_->runInLoop(std::bind(&TcpServer::deleteConnectionInLoop, this, conn));
}

void TcpServer::setMessageCallback(std::function<void(std::shared_ptr<Connection>)> fn) { messageCallback_ = std::move(fn); }

void TcpServer::setConnectionCallback(std::function<void(std::shared_ptr<Connection>)> fn) { connectionCallback_ = std::move(fn); }

void TcpServer::start()
{
    for (size_t i = 0; i < threadPool_->getSize(); ++i)
    {
        threadPool_->add(std::bind(&TcpServer::createEventLoopThread, this));
    }
    mainReactor_->loop();
}

void TcpServer::createEventLoopThread()
{
    EventLoop* reactor = new EventLoop();
    subReactors_.push_back(reactor);
    reactor->loop();
}

void TcpServer::deleteConnectionInLoop(std::shared_ptr<Connection> conn)
{
    int sockfd = conn->getFd();
    connections_.erase(conn->getFd());
}