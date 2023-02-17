#include "Poller.h"
#include "Channel.h"
#include <unistd.h>
#include <string.h>
#include <stdexcept>

Poller::Poller() : epfd_(epoll_create1(EPOLL_CLOEXEC))
{
    if (epfd_ == -1)    throw std::runtime_error("epoll create error");
    events_ = new epoll_event[kMaxevents];
    bzero(events_, sizeof(*events_) * kMaxevents);
}

Poller::~Poller()
{
    close(epfd_);
    delete[] events_;
}

std::vector<Channel *> Poller::poll(int timeout)
{
    std::vector<Channel *> activeChannels;
    int nfds = epoll_wait(epfd_, events_, kMaxevents, timeout);
    if (nfds == -1)     throw std::runtime_error("epoll wait error");
    for (int i = 0; i < nfds; ++i)
    {
        Channel *ch = (Channel *)events_[i].data.ptr;
        ch->setReadyEvents(events_[i].events);
        activeChannels.emplace_back(ch);
    }
    return activeChannels;
}

void Poller::updateChannel(Channel *channel)
{
    int fd = channel->getFd();
    epoll_event ev;
    bzero(&ev, sizeof(ev));
    ev.data.ptr = channel;
    ev.events = channel->getListenEvents();
    if (!channel->getInEpoll())
    {
        if (epoll_ctl(epfd_, EPOLL_CTL_ADD, fd, &ev) == -1)
            throw std::runtime_error("epoll add error");
        channel->setInEpoll();
    }
    else
    {
        if (epoll_ctl(epfd_, EPOLL_CTL_MOD, fd, &ev) == -1)
            throw std::runtime_error("epoll modify error");
    }
}

void Poller::deleteChannel(Channel *channel)
{
    int fd = channel->getFd();
    if (epoll_ctl(epfd_, EPOLL_CTL_DEL, fd, NULL) == -1)
        throw std::runtime_error("epoll delete error");
    channel->setInEpoll(false);
    close(fd);
}