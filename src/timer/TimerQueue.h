#pragma once
#include "Timestamp.h"
#include "Channel.h"
#include "noncopyable.h"
#include <vector>
#include <set>
#include <memory>
#include "Timer.h"
class Timer;

class TimerQueue
{
public:
    DISALLOW_COPY_AND_MOVE(TimerQueue);
    explicit TimerQueue(EventLoop* loop);
    ~TimerQueue();

    // 插入定时器（回调函数，到期时间，是否重复）
    std::shared_ptr<Timer> addTimer(std::function<void()> cb, const Timestamp &when, const double &interval);
    // 删除定时器
    void cancel(std::shared_ptr<Timer> timer);
    
private:
    using Entry = std::pair<Timestamp, std::shared_ptr<Timer>>; // 以时间戳作为键值获取定时器

    // 定时器读事件触发的函数
    void handleRead();

    // 重新设置timerfd_
    void resetTimerfd(int timerfd_, Timestamp expiration);
    
    // 移除所有已到期的定时器
    // 1.获取到期的定时器
    // 2.重置这些定时器（销毁或者重复定时任务）
    std::vector<Entry> getExpired(Timestamp now);
    void reset(const std::vector<Entry>& expired, Timestamp now);

    // 插入定时器的内部方法
    bool insert(std::shared_ptr<Timer> timer);

    EventLoop* loop_; 
    int timerfd_;
    std::unique_ptr<Channel> timerfdChannel_;
    std::set<Entry> timers_;
};
