#include "Channel.h"
#include <sys/epoll.h>
#include <unistd.h>
#include <utility>
#include "EventLoop.h"
#include "Socket.h"

Channel::Channel(EventLoop *loop, int fd)
    : loop_(loop), fd_(fd), listenEvents_(0), readyEvents_(0), inEpoll_(false) {}

Channel::~Channel()
{
    if (fd_ != -1)
    {
        close(fd_);
        fd_ = -1;
    }
}

void Channel::handleEvent()
{
    if (readyEvents_ & (EPOLLIN | EPOLLPRI))
    {
        readCallback_();
    }
    if (readyEvents_ & (EPOLLOUT))
    {
        writeCallback_();
    }
}

void Channel::enableRead()
{
    listenEvents_ |= EPOLLIN | EPOLLPRI;
    loop_->updateChannel(this);
}

void Channel::useET()
{
    listenEvents_ |= EPOLLET;
    loop_->updateChannel(this);
}
int Channel::getFd() { return fd_; }

uint32_t Channel::getListenEvents() { return listenEvents_; }
uint32_t Channel::getReadyEvents() { return readyEvents_; }

bool Channel::getInEpoll() { return inEpoll_; }

void Channel::setInEpoll(bool in) { inEpoll_ = in; }

void Channel::setReadyEvents(uint32_t ev) { readyEvents_ = ev; }

void Channel::setReadCallback(std::function<void()> const &callback) { readCallback_ = callback; }

void Channel::setWriteCallback(std::function<void()> const &callback) { writeCallback_ = callback; }