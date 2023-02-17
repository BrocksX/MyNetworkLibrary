#pragma once
#include "noncopyable.h"
#include <functional>
#include "Timestamp.h"
#include "TimerQueue.h"

class TimerQueue;

class Timer
{
    friend class TimerQueue;
private:
    std::function<void()> callback_;
    Timestamp expiration_;
    double interval_;
    bool repeat_;
    bool disabled_;
    void restart(Timestamp now);
    void run() const;

public:
    Timer(std::function<void()> cb, Timestamp time, double interval);
    ~Timer() = default;

    Timestamp getExpiration() const;
    bool isRepeat() const;
};

