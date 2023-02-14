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

std::shared_ptr<Timer> EventLoop::runAt(const Timestamp &timestamp, std::function<void()>&& cb)
{
    return timerQueue_->addTimer(std::move(cb), timestamp, 0.0);
}

std::shared_ptr<Timer> EventLoop::runAfter(const double &waitTime, std::function<void()>&& cb)
{
    Timestamp time(addTime(Timestamp::now(), waitTime)); 
    return runAt(time, std::move(cb));
}

std::shared_ptr<Timer> EventLoop::runEvery(const double &interval, std::function<void()>&& cb)
{
    Timestamp timestamp(addTime(Timestamp::now(), interval)); 
    return timerQueue_->addTimer(std::move(cb), timestamp, interval);
}

void EventLoop::cancel(std::shared_ptr<Timer> timer)
{
    timerQueue_->cancel(timer);
}