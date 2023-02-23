#include "EventLoop.h"
#include "Poller.h"
#include "Channel.h"
#include <vector>
#include "ThreadPool.h"

__thread EventLoop *t_loopInThisThread = nullptr;


EventLoop::EventLoop() : quit_(false), poller_(std::make_unique<Poller>()), timerQueue_(std::make_unique<TimerQueue>(this)), 
threadId_(CurrentThread::tid()), callingPendingFunctors_(false), wakeupFd_(::eventfd(0, EFD_NONBLOCK | EFD_CLOEXEC))
{
    if (wakeupFd_ < 0)
        throw std::runtime_error("create wakeup fd error");
    wakeupChannel_ = std::make_unique<Channel>(this, wakeupFd_);
    wakeupChannel_->setReadCallback(std::bind(&EventLoop::handleRead, this));
    wakeupChannel_->enableReading();
    if (t_loopInThisThread)
        throw std::runtime_error("this thread already has a EventLoop");
    else
        t_loopInThisThread = this;
}

EventLoop::~EventLoop()
{
    wakeupChannel_->disableAll();
    wakeupChannel_->remove();
    t_loopInThisThread = nullptr;
}

void EventLoop::loop()
{
    while(!quit_)
    {
        activeChannels_.clear();
        activeChannels_ = poller_->poll();
        for(Channel *ch : activeChannels_)
        {
            ch->handleEvent();
        }
        doPendingFunctors();
    }
}

void EventLoop::runInLoop(Functor function)
{
    if(isInLoopThread())
    {
        function();
    }
    else
        queueInLoop(function);
}

void EventLoop::queueInLoop(Functor function)
{
    {
        std::unique_lock<std::mutex> lock(mutex_);
        pendingFunctors_.emplace_back(function);
    }
    if (!isInLoopThread() || callingPendingFunctors_)
        wakeup();
}

void EventLoop::wakeup()
{
    uint16_t one = 1;
    ssize_t n = write(wakeupFd_, &one, sizeof(one));
}

void EventLoop::handleRead()
{
    uint16_t one = 1;
    ssize_t n = read(wakeupFd_, &one, sizeof(one));
}

void EventLoop::doPendingFunctors()
{
    std::vector<Functor> functors;
    callingPendingFunctors_ = true;

    {
        std::unique_lock<std::mutex> lock(mutex_);
        functors.swap(pendingFunctors_);
    }

    for (const Functor &functor : functors)
        functor();
    callingPendingFunctors_ = false;
}

void EventLoop::quit()
{
    quit_ = true;
    if (isInLoopThread())
    {
        wakeup();
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

std::shared_ptr<Timer> EventLoop::runAt(const Timestamp &timestamp, Functor&& cb)
{
    return timerQueue_->addTimer(std::move(cb), timestamp, 0.0);
}

std::shared_ptr<Timer> EventLoop::runAfter(const double &waitTime, Functor&& cb)
{
    Timestamp time(addTime(Timestamp::now(), waitTime)); 
    return runAt(time, std::move(cb));
}

std::shared_ptr<Timer> EventLoop::runEvery(const double &interval, Functor&& cb)
{
    Timestamp timestamp(addTime(Timestamp::now(), interval)); 
    return timerQueue_->addTimer(std::move(cb), timestamp, interval);
}

void EventLoop::cancel(std::shared_ptr<Timer> timer)
{
    timerQueue_->cancel(timer);
}