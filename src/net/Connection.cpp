#include "Connection.h"
#include <unistd.h>
#include <cassert>
#include <cstring>
#include <iostream>
#include <utility>
#include "Buffer.h"
#include "Channel.h"
#include "Socket.h"

Connection::Connection(EventLoop *loop, Socket *socket) : socket_(std::unique_ptr<Socket>(socket)), loop_(loop)
{
    if (loop == nullptr)
        throw std::runtime_error("socket loop nullptr");
    readBuffer_ = std::make_unique<Buffer>();
    sendBuffer_ = std::make_unique<Buffer>();
    channel_ = std::make_unique<Channel>(loop_, socket_->getFd());
    fd_ = socket_->getFd();
}

Connection::~Connection() 
{
}

void Connection::connectEstablished()
{
    channel_->enableReading();
    state_ = State::Connected;
}

void Connection::connectDestroyed()
{
    if (state_ == Connected)
    {
        state_ = Closed;
        channel_->disableAll();
    }
    channel_->remove();
}

void Connection::read()
{
    assert(state_ == State::Connected);
    readBuffer_->clear();
    if (socket_->isNonBlocking())
    {
        readNonBlocking();
    }
    else
    {
        readBlocking();
    }
}
void Connection::write()
{
    assert(state_ == State::Connected);
    if (socket_->isNonBlocking())
    {
        writeNonBlocking();
    }
    else
    {
        writeBlocking();
    }
    sendBuffer_->clear();
}

void Connection::readNonBlocking()
{
    int sockfd = socket_->getFd();
    char buf[1024];
    while (true)
    {
        // 使用非阻塞IO，读取客户端buffer，一次读取buf大小数据，直到全部读取完毕
        memset(buf, 0, sizeof(buf));
        ssize_t bytes_read = ::read(sockfd, buf, sizeof(buf));
        if (bytes_read > 0)
        {
            readBuffer_->append(buf, bytes_read);
        }
        else if (bytes_read == -1 && errno == EINTR)
        {
            // 程序正常中断、继续读取
            continue;
        }
        else if (bytes_read == -1 && ((errno == EAGAIN) || (errno == EWOULDBLOCK)))
        {
            // 非阻塞IO，这个条件表示数据全部读取完毕
            break;
        }
        else if (bytes_read == 0)
        {
            // EOF，客户端断开连接
            channel_->disableAll();
            state_ = State::Closed;
            printf("Connection fd %d disconnected\n", sockfd);
            break;
        }
        else
        {
            printf("Other error on client fd %d\n", sockfd);
            channel_->disableAll();
            state_ = State::Closed;
            break;
        }
    }
}
void Connection::writeNonBlocking()
{
    int sockfd = socket_->getFd();
    char buf[sendBuffer_->size()];
    memcpy(buf, sendBuffer_->c_str(), sendBuffer_->size());
    int data_size = sendBuffer_->size();
    int data_left = data_size;
    while (data_left > 0)
    {
        ssize_t bytes_write = ::write(sockfd, buf + data_size - data_left, data_left);
        if (bytes_write == -1 && errno == EINTR)
        {
            printf("continue writing\n");
            continue;
        }
        if (bytes_write == -1 && errno == EAGAIN)
        {
            break;
        }
        if (bytes_write == -1)
        {
            printf("Other error on client fd %d\n", sockfd);
            state_ = State::Closed;
            break;
        }
        data_left -= bytes_write;
    }
}

void Connection::readBlocking()
{
    int sockfd = socket_->getFd();
    unsigned int rcv_size = 0;
    socklen_t len = sizeof(rcv_size);
    getsockopt(sockfd, SOL_SOCKET, SO_RCVBUF, &rcv_size, &len);
    char buf[rcv_size];
    ssize_t bytes_read = ::read(sockfd, buf, sizeof(buf));
    if (bytes_read > 0)
    {
        readBuffer_->append(buf, bytes_read);
    }
    else if (bytes_read == 0)
    {
        printf("blocking client fd %d disconnected\n", sockfd);
        channel_->disableAll();
        state_ = State::Closed;
    }
    else if (bytes_read == -1)
    {
        printf("Other error on blocking client fd %d\n", sockfd);
        channel_->disableAll();
        state_ = State::Closed;
    }
}

void Connection::writeBlocking()
{
    // 没有处理send_buffer_数据大于TCP写缓冲区的情况，可能会有bug
    int sockfd = socket_->getFd();
    ssize_t bytes_write = ::write(sockfd, sendBuffer_->c_str(), sendBuffer_->size());
    if (bytes_write == -1)
    {
        printf("Other error on blocking client fd %d\n", sockfd);
        state_ = State::Closed;
    }
}

void Connection::close() { deleteConnectioinCallback_(shared_from_this()); }

Connection::State Connection::getState() { return state_; }

void Connection::setSendBuffer(const char *str) { sendBuffer_->setBuf(str); }

Buffer *Connection::getReadBuffer() { return readBuffer_.get(); }

const char *Connection::readBuffer() { return readBuffer_->c_str(); }

Buffer *Connection::getSendBuffer() { return sendBuffer_.get(); }

const char *Connection::sendBuffer() { return sendBuffer_->c_str(); }

void Connection::setDeleteConnectionCallback(std::function<void(std::shared_ptr<Connection>)> const &callback)
{
    deleteConnectioinCallback_ = callback;
}
void Connection::setMessageCallback(std::function<void(std::shared_ptr<Connection>)> const &callback)
{
    messageCallback_ = callback;
    channel_->setReadCallback([this](){ messageCallback_(shared_from_this()); });
}

void Connection::getlineSendBuffer() { sendBuffer_->getline(); }

Socket *Connection::getSocket() { return socket_.get(); }

void Connection::send(const std::string &msg)
{
    loop_->runInLoop(std::bind(&Connection::sendInLoop, this, msg.c_str()));
}

void Connection::send(const Buffer &buffer)
{
    loop_->runInLoop(std::bind(&Connection::sendInLoop, this, buffer.c_str()));
}

void Connection::send(const char *msg)
{
    loop_->runInLoop(std::bind(&Connection::sendInLoop, this, msg));
}

void Connection::sendInLoop(const char *str)
{
    setSendBuffer(str);
    write();
}

void Connection::shutdown()
{
    state_ = Closed;
    loop_->runInLoop([this]() { socket_->shutdownWrite(); });
}