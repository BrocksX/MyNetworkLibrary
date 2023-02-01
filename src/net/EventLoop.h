#pragma once
#include <functional>
#include <memory>
#include <vector>
#include "nocopyable.h"
#include "Timestamp.h"
#include "TimerQueue.h"

class Poller;
class Channel;
class Timestamp;
class TimerQueue;

class EventLoop
{
public:
    DISALLOW_COPY_AND_MOVE(EventLoop);
    EventLoop();
    ~EventLoop();

    void loop();
    void updateChannel(Channel *) const;
    void deleteChannel(Channel *) const;

    void runAt(Timestamp timestamp, std::function<void()>&& cb);
    void runAfter(double waitTime, std::function<void()>&& cb);
    void runEvery(double interval, std::function<void()>&& cb);

private:
    std::unique_ptr<Poller> poller_;
    std::vector<Channel*> activeChannels_;
    std::unique_ptr<TimerQueue> timerQueue_;
    bool quit;

};