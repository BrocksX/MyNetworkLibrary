#include "Poller.h"
#include "util.h"
#include "Channel.h"
#include <unistd.h>
#include <string.h>

#define MAX_EVENTS 1000

Poller::Poller() : epfd_(-1), events_(nullptr)
{
    epfd_ = epoll_create1(EPOLL_CLOEXEC);
    errif(epfd_ == -1, "epoll create error");
    events_ = new epoll_event[MAX_EVENTS];
    bzero(events_, sizeof(*events_) * MAX_EVENTS);
}
Poller::~Poller()
{
    if (epfd_ != -1)
    {
        close(epfd_);
        epfd_ = -1;
    }
    delete[] events_;
}
std::vector<Channel *> Poller::poll(int timeout)
{
    std::vector<Channel *> activeChannels;
    int nfds = epoll_wait(epfd_, events_, MAX_EVENTS, timeout);
    errif(nfds == -1, "epoll wait error");
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
        errif(epoll_ctl(epfd_, EPOLL_CTL_ADD, fd, &ev) == -1, "epoll add error");
        channel->setInEpoll();
    }
    else
    {
        errif(epoll_ctl(epfd_, EPOLL_CTL_MOD, fd, &ev) == -1, "epoll modify error");
    }
}
void Poller::deleteChannel(Channel* channel)
{
    int fd = channel->getFd();
    errif(epoll_ctl(epfd_, EPOLL_CTL_DEL, fd, NULL)==-1, "epoll delete error");
    channel->setInEpoll(false);
    close(fd);
}