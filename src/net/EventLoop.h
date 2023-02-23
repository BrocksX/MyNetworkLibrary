#pragma once
#include <functional>
#include <memory>
#include <vector>
#include <mutex>
#include <atomic>
#include <sys/eventfd.h> 
#include "noncopyable.h"
#include "Timestamp.h"
#include "TimerQueue.h"
#include "Timer.h"
#include "CurrentThread.h"

class Poller;
class Channel;
class Timestamp;
class Timer;
class TimerQueue;
/**
 * EventLoop是事件循环类，将需要监听的事务添加到这个事件循环内，每次有事件发生时就会通知，然后根据不同的描述符、事件类型进行处理。
*/
class EventLoop
{
public:
    using Functor = std::function<void()>;
    
    DISALLOW_COPY_AND_MOVE(EventLoop);
    EventLoop();
    ~EventLoop();

    void loop();
    void quit();

    void runInLoop(Functor function);
    void queueInLoop(Functor function);
    bool isInLoopThread() const { return threadId_ == CurrentThread::tid(); }

    void updateChannel(Channel *) const;
    void deleteChannel(Channel *) const;

    //定时器相关功能
    std::shared_ptr<Timer> runAt(const Timestamp &timestamp, Functor&& cb);
    std::shared_ptr<Timer> runAfter(const double &waitTime, Functor&& cb);
    std::shared_ptr<Timer> runEvery(const double &interval, Functor&& cb);
    void cancel(std::shared_ptr<Timer> timer);

private:
    std::unique_ptr<Poller> poller_;
    std::vector<Channel*> activeChannels_;
    std::unique_ptr<TimerQueue> timerQueue_;
    bool quit_;

    const pid_t threadId_;
    std::mutex mutex_;
    std::vector<Functor> pendingFunctors_;
    const int wakeupFd_; 
    std::atomic<bool> callingPendingFunctors_;
    std::unique_ptr<Channel> wakeupChannel_;

    void wakeup();
    void handleRead();
    void doPendingFunctors();
};