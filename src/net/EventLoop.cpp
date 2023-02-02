#include "EventLoop.h"
#include "Poller.h"
#include "Channel.h"
#include <vector>
#include "ThreadPool.h"

EventLoop::EventLoop() : quit(false), poller_(std::make_unique<Poller>()), timerQueue_(std::make_unique<TimerQueue>(this)){}

EventLoop::~EventLoop(){}

void EventLoop::loop()
{
    while(!quit)
    {
        activeChannels_.clear();
        activeChannels_ = poller_->poll();
        for(Channel *ch : activeChannels_)
        {
            ch->handleEvent();
        }
    }
}
void EventLoop::updateChannel(Channel* ch) const
{
    poller_->updateChannel(ch);
}
void EventLoop::deleteChannel(Channel *ch) const
{
    poller_->deleteChannel(ch);
}

void EventLoop::runAt(Timestamp timestamp, std::function<void()>&& cb)
{
    timerQueue_->addTimer(std::move(cb), timestamp, 0.0);
}

void EventLoop::runAfter(double waitTime, std::function<void()>&& cb)
{
    Timestamp time(addTime(Timestamp::now(), waitTime)); 
    runAt(time, std::move(cb));
}

void EventLoop::runEvery(double interval, std::function<void()>&& cb)
{
    Timestamp timestamp(addTime(Timestamp::now(), interval)); 
    timerQueue_->addTimer(std::move(cb), timestamp, interval);
}