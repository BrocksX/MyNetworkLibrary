#include "Channel.h"
#include <unistd.h>
#include <utility>
#include "EventLoop.h"
#include "Socket.h"


Channel::Channel(EventLoop *loop, int fd) : loop_(loop), fd_(fd), listenEvents_(0), readyEvents_(0), inEpoll_(false) {}

Channel::~Channel()
{
    if(fd_ != -1)   close(fd_);
}

void Channel::handleEvent()
{
    if (readyEvents_ & kReadEvent)
    {
        readCallback_();
    }
    if (readyEvents_ & kWriteEvent)
    {
        writeCallback_();
    }
}

void Channel::enableReading()
{
    listenEvents_ |= kReadEvent;
    loop_->updateChannel(this);
}

void Channel::useET()
{
    listenEvents_ |= EPOLLET;
    loop_->updateChannel(this);
}

void Channel::disableAll() 
{ 
    listenEvents_ &= kNoneEvent; 
    loop_->updateChannel(this);
}

void Channel::remove()
{
    loop_->deleteChannel(this);
}

int Channel::getFd() const { return fd_; }

uint32_t Channel::getListenEvents() const { return listenEvents_; }

uint32_t Channel::getReadyEvents() const{ return readyEvents_; }

bool Channel::getInEpoll() const { return inEpoll_; }

void Channel::setInEpoll(bool in) { inEpoll_ = in; }

void Channel::setReadyEvents(const uint32_t &ev) { readyEvents_ = ev; }

void Channel::setReadCallback(std::function<void()> const &callback) { readCallback_ = callback; }

void Channel::setWriteCallback(std::function<void()> const &callback) { writeCallback_ = callback; }