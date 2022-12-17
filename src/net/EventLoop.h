#pragma once
#include <functional>
#include"common.h"
class EventLoop
{
private:
    Epoll *ep_;
    bool quit;
public:
    DISALLOW_COPY_AND_MOVE(EventLoop);
    EventLoop();
    ~EventLoop();

    void loop() const;
    void updateChannel(Channel *) const;
};