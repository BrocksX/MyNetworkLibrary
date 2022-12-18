#include "EventLoop.h"
#include "Poller.h"
#include "Channel.h"
#include <vector>
#include "ThreadPool.h"

EventLoop::EventLoop() : poller_(nullptr), quit(false)
{
    poller_ = std::make_unique<Poller>();
}
EventLoop::~EventLoop(){}

void EventLoop::loop() const
{
    while(!quit)
    {
        std::vector<Channel*> chs;
        chs = poller_->poll();
        for(auto it = chs.begin(); it!=chs.end();++it)
        {
            (*it)->handleEvent();
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