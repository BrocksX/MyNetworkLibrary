#include "Connection.h"
#include <unistd.h>
#include <cassert>
#include <cstring>
#include <iostream>
#include <utility>
#include "Buffer.h"
#include "Channel.h"
#include "Socket.h"

Connection::Connection(EventLoop *loop, Socket *socket) : socket_(socket)
{
    if (loop != nullptr)
    {
        channel_ = std::make_unique<Channel>(loop, socket->getFd());
        channel_->enableReading();
        channel_->useET();
    }
    readBuffer_ = std::make_unique<Buffer>();
    sendBuffer_ = std::make_unique<Buffer>();
    state_ = State::Connected;
}

Connection::~Connection() {}

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
        //使用非阻塞IO，读取客户端buffer，一次读取buf大小数据，直到全部读取完毕
        memset(buf, 0, sizeof(buf));
        ssize_t bytes_read = ::read(sockfd, buf, sizeof(buf));
        if (bytes_read > 0)
        {
            readBuffer_->append(buf, bytes_read);
        }
        else if (bytes_read == -1 && errno == EINTR)
        { 
            //程序正常中断、继续读取
            continue;
        }
        else if (bytes_read == -1 && ((errno == EAGAIN) || (errno == EWOULDBLOCK)))
        { 
            //非阻塞IO，这个条件表示数据全部读取完毕
            break;
        }
        else if (bytes_read == 0)
        { 
            // EOF，客户端断开连接
            printf("client fd %d disconnected\n", sockfd);
            state_ = State::Closed;
            break;
        }
        else
        {
            printf("Other error on client fd %d\n", sockfd);
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
        state_ = State::Closed;
    }
    else if (bytes_read == -1)
    {
        printf("Other error on blocking client fd %d\n", sockfd);
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

void Connection::close() { deleteConnectioinCallback_(socket_.get()); }

Connection::State Connection::getState() { return state_; }

void Connection::setSendBuffer(const char *str) { sendBuffer_->setBuf(str); }

Buffer *Connection::getReadBuffer() { return readBuffer_.get(); }

const char *Connection::readBuffer() { return readBuffer_->c_str(); }

Buffer *Connection::getSendBuffer() { return sendBuffer_.get(); }

const char *Connection::sendBuffer() { return sendBuffer_->c_str(); }

void Connection::setDeleteConnectionCallback(std::function<void(Socket *)> const &callback)
{
    deleteConnectioinCallback_ = callback;
}
void Connection::setMessageCallback(std::function<void(Connection *)> const &callback)
{
    messageCallback_ = callback;
    channel_->setReadCallback([this](){ messageCallback_(this); });
}

void Connection::getlineSendBuffer() { sendBuffer_->getline(); }

Socket *Connection::getSocket() { return socket_.get(); }

void Connection::send(const std::string &msg)
{
    setSendBuffer(msg.c_str());
    write();
}

void Connection::send(const Buffer &buffer)
{
    setSendBuffer(buffer.c_str());
    write();
}