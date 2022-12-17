#pragma once
#include <sys/epoll.h>
#include <vector>
#include "common.h"
class Epoll
{
private:
    int epfd_;
    epoll_event *events_;
public:
    DISALLOW_COPY_AND_MOVE(Epoll);
    Epoll();
    ~Epoll();

    void updateChannel(Channel*);
    void deleteChannel(Channel*);
    std::vector<Channel*> poll(int timeout = -1);
};
