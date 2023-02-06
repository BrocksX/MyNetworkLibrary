#include "EventLoop.h"
#include "Channel.h"
#include "Timer.h"
#include "TimerQueue.h"
#include <sys/timerfd.h>
#include <unistd.h>
#include <string.h>
#include <util.h>

TimerQueue::TimerQueue(EventLoop* loop) : loop_(loop), timerfdChannel_(loop_, timerfd_), timers_()
{
    timerfd_ = ::timerfd_create(CLOCK_MONOTONIC, TFD_NONBLOCK | TFD_CLOEXEC);
    errif(timerfd_ < 0, "Failed in timerfd_create");
    timerfdChannel_.setReadCallback(std::bind(&TimerQueue::handleRead, this));
    timerfdChannel_.enableRead();
}

TimerQueue::~TimerQueue()
{   

    ::close(timerfd_);
    for (const Entry& timer : timers_)
    {
        delete timer.second;
    }
}

Timer* TimerQueue::addTimer(std::function<void()> cb, Timestamp when, double interval)
{
    Timer* timer = new Timer(std::move(cb), when, interval);
    addTimerInLoop(timer);
    return timer;
}

void TimerQueue::addTimerInLoop(Timer* timer)
{
    // 是否取代了最早的定时触发时间
    bool eraliestChanged = insert(timer);

    if (eraliestChanged)
    {
        resetTimerfd(timerfd_, timer->getExpiration());
    }
}

void TimerQueue::cancel(Timer *timer)
{
    Entry t = {timer->getExpiration(), timer};
    if(std::set<Entry>::iterator it = timers_.find(t); it != timers_.end())
    {
        timers_.erase(it);
        delete it->second;
    }
}

// 重置timerfd
void TimerQueue::resetTimerfd(int timerfd_, Timestamp expiration)
{
    struct itimerspec newValue;
    struct itimerspec oldValue;
    memset(&newValue, '\0', sizeof(newValue));
    memset(&oldValue, '\0', sizeof(oldValue));

    // 超时时间 - 现在时间
    int64_t microSecondDif = expiration.getMicroSecondsSinceEpoch() - Timestamp::now().getMicroSecondsSinceEpoch();
    if (microSecondDif < 100)
    {
        microSecondDif = 100;
    }
    struct timespec ts;
    ts.tv_sec = static_cast<time_t>(microSecondDif / Timestamp::kMicroSecondsPerSecond);
    ts.tv_nsec = static_cast<long>((microSecondDif % Timestamp::kMicroSecondsPerSecond) * 1000);
    newValue.it_value = ts;
    // 此函数会唤醒事件循环
    ::timerfd_settime(timerfd_, 0, &newValue, &oldValue);
}

void ReadTimerFd(int timerfd) 
{
    uint64_t read_byte;
    ssize_t readn = ::read(timerfd, &read_byte, sizeof(read_byte));
    
    if (readn != sizeof(read_byte)) {
        //LOG_ERROR << "TimerQueue::ReadTimerFd read_size < 0";
    }
}

std::vector<TimerQueue::Entry> TimerQueue::getExpired(Timestamp now)
{
    std::vector<Entry> expired;
    // TODO:???
    Entry sentry(now, reinterpret_cast<Timer*>(UINTPTR_MAX));
    std::set<Entry>::iterator end = timers_.lower_bound(sentry);
    std::copy(timers_.begin(), end, back_inserter(expired));
    timers_.erase(timers_.begin(), end);
    
    return expired;
}

void TimerQueue::handleRead()
{
    Timestamp now = Timestamp::now();
    ReadTimerFd(timerfd_);

    std::vector<Entry> expired = getExpired(now);

    // 遍历到期的定时器，调用回调函数
    callingExpiredTimers_ = true;
    for (const Entry& it : expired)
    {
        it.second->run();
    }
    callingExpiredTimers_ = false;
    reset(expired, now);

}

void TimerQueue::reset(const std::vector<Entry>& expired, Timestamp now)
{
    Timestamp nextExpire;
    for (const Entry& it : expired)
    {
        // 重复任务则继续执行
        if (it.second->isRepeat())
        {
            auto timer = it.second;
            timer->restart(Timestamp::now());
            insert(timer);
        }
        else
        {
            delete it.second;
        }
        // 如果重新插入了定时器，需要继续重置timerfd
        if (!timers_.empty())
        {
            resetTimerfd(timerfd_, (timers_.begin()->second)->getExpiration());
        }
    }
}

bool TimerQueue::insert(Timer* timer)
{
    bool earliestChanged = false;
    Timestamp when = timer->getExpiration();
    std::set<Entry>::iterator it = timers_.begin();
    if (it == timers_.end() || when < it->first)
    {
        // 说明最早的定时器已经被替换了
        earliestChanged = true;
    }
    timers_.insert(Entry(when, timer));

    return earliestChanged;
}


