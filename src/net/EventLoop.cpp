#include "EventLoop.h"
#include "Epoll.h"
#include "Channel.h"
#include <vector>
#include "ThreadPool.h"

EventLoop::EventLoop() : ep_(nullptr), quit(false)
{
    ep_ = new Epoll();
}
EventLoop::~EventLoop()
{
    delete ep_;
}
void EventLoop::loop() const
{
    while(!quit)
    {
        std::vector<Channel*> chs;
        chs = ep_->poll();
        for(auto it = chs.begin(); it!=chs.end();++it)
        {
            (*it)->handleEvent();
        }
    }
}
void EventLoop::updateChannel(Channel* ch) const
{
    ep_->updateChannel(ch);
}