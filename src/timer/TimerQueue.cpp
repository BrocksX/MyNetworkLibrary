#include "EventLoop.h"
#include "Channel.h"
#include "Timer.h"
#include "TimerQueue.h"
#include <sys/timerfd.h>
#include <unistd.h>
#include <string.h>

TimerQueue::TimerQueue(EventLoop* loop) : loop_(loop)
{
    timerfd_ = ::timerfd_create(CLOCK_MONOTONIC, TFD_NONBLOCK | TFD_CLOEXEC);
    if(timerfd_ <= 0)   throw std::runtime_error("Failed in timerfd_create");
    timerfdChannel_ = std::make_unique<Channel>(loop_, timerfd_);
    timerfdChannel_->setReadCallback(std::bind(&TimerQueue::handleRead, this));
    timerfdChannel_->enableReading();
}

TimerQueue::~TimerQueue()
{   
    ::close(timerfd_);
}

std::shared_ptr<Timer> TimerQueue::addTimer(std::function<void()> cb, const Timestamp &when, const double &interval)
{
    std::shared_ptr<Timer> timer = std::make_shared<Timer>(std::move(cb), when, interval);
    loop_->runInLoop([this, &timer](){
        bool eraliestChanged = this->insert(timer);
        if (eraliestChanged)
        {
            resetTimerfd(timerfd_, timer->getExpiration());
        }
    });
    return timer;
}

void TimerQueue::cancel(std::shared_ptr<Timer>timer)
{
    loop_->runInLoop([this, &timer](){timer->disabled_ = true;});
}

void TimerQueue::resetTimerfd(int timerfd_, Timestamp expiration)
{
    itimerspec newValue;
    memset(&newValue, '\0', sizeof(newValue));

    int64_t microSecondDif = expiration.getMicroSecondsSinceEpoch() - Timestamp::now().getMicroSecondsSinceEpoch();
    if (microSecondDif < 100)
    {
        microSecondDif = 100;
    }
    timespec ts;
    ts.tv_sec = static_cast<time_t>(microSecondDif / Timestamp::kMicroSecondsPerSecond);
    ts.tv_nsec = static_cast<long>((microSecondDif % Timestamp::kMicroSecondsPerSecond) * 1000);
    newValue.it_value = ts;
    ::timerfd_settime(timerfd_, 0, &newValue, 0);
}

void ReadTimerFd(int timerfd) 
{
    uint64_t read_byte;
    ssize_t readn = ::read(timerfd, &read_byte, sizeof(read_byte));
}

std::vector<TimerQueue::Entry> TimerQueue::getExpired(Timestamp now)
{
    std::vector<Entry> expired;
    while(!timers_.empty() && timers_.top().first < now)
    {
        expired.emplace_back(timers_.top());
        timers_.pop();
    }
    return expired;
}

void TimerQueue::handleRead()
{
    Timestamp now = Timestamp::now();
    ReadTimerFd(timerfd_);
    std::vector<Entry> expired = getExpired(now);

    for (const Entry& it : expired)
    {
        if(!it.second->disabled_)
            it.second->run();
    }
    reset(expired, now);
}

void TimerQueue::reset(const std::vector<Entry>& expired, Timestamp now)
{
    for (const Entry& it : expired)
    {
        // 重复任务且任务未取消则继续执行
        if (it.second->isRepeat() && !it.second->disabled_)
        {
            auto timer = it.second;
            timer->restart(Timestamp::now());
            timers_.push(Entry(timer->getExpiration(), timer));
        }
    }
    if (!timers_.empty())
    {
        resetTimerfd(timerfd_, (timers_.top().second)->getExpiration());
    }
}

bool TimerQueue::insert(std::shared_ptr<Timer> timer)
{
    bool earliestChanged = false;
    Timestamp when = timer->getExpiration();
    if(timers_.empty() || when < timers_.top().first)
        earliestChanged = true;
    timers_.push(Entry(when, timer));
    return earliestChanged;
}


