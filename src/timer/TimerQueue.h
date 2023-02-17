#pragma once
#include "Timestamp.h"
#include "Channel.h"
#include "noncopyable.h"
#include <vector>
#include <set>
#include <memory>
#include <queue>
#include "Timer.h"
class Timer;

class TimerQueue
{
public:
    DISALLOW_COPY_AND_MOVE(TimerQueue);
    explicit TimerQueue(EventLoop* loop);
    ~TimerQueue();

    // 插入定时器
    std::shared_ptr<Timer> addTimer(std::function<void()> cb, const Timestamp &when, const double &interval);
    // 删除定时器
    void cancel(std::shared_ptr<Timer> timer);
    
private:
    using Entry = std::pair<Timestamp, std::shared_ptr<Timer>>;
    EventLoop* loop_; 
    int timerfd_;
    std::unique_ptr<Channel> timerfdChannel_;
    std::priority_queue<Entry, std::vector<Entry>, std::greater<Entry>> timers_;

    void handleRead();

    // 重新设置timerfd_
    void resetTimerfd(int timerfd_, Timestamp expiration);
    
    // 获取到期的定时器
    std::vector<Entry> getExpired(Timestamp now);
    void reset(const std::vector<Entry>& expired, Timestamp now);

    bool insert(std::shared_ptr<Timer> timer);
};
