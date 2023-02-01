#pragma once
#include <sys/epoll.h>
#include <vector>
#include "nocopyable.h"

class Channel;

class Poller
{
public:
    DISALLOW_COPY_AND_MOVE(Poller);
    Poller();
    ~Poller();

    void updateChannel(Channel*);
    void deleteChannel(Channel*);
    std::vector<Channel*> poll(int timeout = -1);

private:
    int epfd_;
    epoll_event *events_;
};
