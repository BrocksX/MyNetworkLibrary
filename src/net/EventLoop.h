#pragma once
#include <functional>
#include"common.h"
class EventLoop
{
private:
    std::unique_ptr<Poller>poller_;
    bool quit;
public:
    DISALLOW_COPY_AND_MOVE(EventLoop);
    EventLoop();
    ~EventLoop();

    void loop() const;
    void updateChannel(Channel *) const;
    void deleteChannel(Channel *) const;
};