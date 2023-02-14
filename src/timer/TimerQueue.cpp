#include "EventLoop.h"
#include "Channel.h"
#include "Timer.h"
#include "TimerQueue.h"
#include <sys/timerfd.h>
#include <unistd.h>
#include <string.h>
#include <util.h>

TimerQueue::TimerQueue(EventLoop* loop) : loop_(loop)
{
    timerfd_ = ::timerfd_create(CLOCK_MONOTONIC, TFD_NONBLOCK | TFD_CLOEXEC);
    errif(timerfd_ <= 0, "Failed in timerfd_create");
    timerfdChannel_ = std::make_unique<Channel>(loop_, timerfd_);
    timerfdChannel_->setReadCallback(std::bind(&TimerQueue::handleRead, this));
    timerfdChannel_->enableRead();
}

TimerQueue::~TimerQueue()
{   
    ::close(timerfd_);
}

std::shared_ptr<Timer> TimerQueue::addTimer(std::function<void()> cb, const Timestamp &when, const double &interval)
{
    std::shared_ptr<Timer> timer = std::make_shared<Timer>(std::move(cb), when, interval);
    bool eraliestChanged = insert(timer);

    if (eraliestChanged)
    {
        resetTimerfd(timerfd_, timer->getExpiration());
    }
    return timer;
}

void TimerQueue::cancel(std::shared_ptr<Timer>timer)
{
    timer->disable();
}

// 重置timerfd
void TimerQueue::resetTimerfd(int timerfd_, Timestamp expiration)
{
    itimerspec newValue;
    memset(&newValue, '\0', sizeof(newValue));

    // 超时时间 - 现在时间
    int64_t microSecondDif = expiration.getMicroSecondsSinceEpoch() - Timestamp::now().getMicroSecondsSinceEpoch();
    if (microSecondDif < 100)
    {
        microSecondDif = 100;
    }
    timespec ts;
    ts.tv_sec = static_cast<time_t>(microSecondDif / Timestamp::kMicroSecondsPerSecond);
    ts.tv_nsec = static_cast<long>((microSecondDif % Timestamp::kMicroSecondsPerSecond) * 1000);
    newValue.it_value = ts;
    // 此函数会唤醒事件循环
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
    Entry sentry(now, std::make_shared<Timer>([](){}, now, 0));
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
    for (const Entry& it : expired)
    {
        if(!it.second->isValid())
            it.second->run();
    }
    reset(expired, now);
}

void TimerQueue::reset(const std::vector<Entry>& expired, Timestamp now)
{
    for (const Entry& it : expired)
    {
        // 重复任务则继续执行
        if (it.second->isRepeat() && !it.second->isValid())
        {
            auto timer = it.second;
            timer->restart(Timestamp::now());
            insert(timer);
        }
    }
    if (!timers_.empty())
    {
        resetTimerfd(timerfd_, (timers_.begin()->second)->getExpiration());
    }
}

bool TimerQueue::insert(std::shared_ptr<Timer> timer)
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


