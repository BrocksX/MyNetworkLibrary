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

    void disable()
    {
        disabled_ = true;
    }
    bool isValid()
    {
        return disabled_;
    }
    void restart(Timestamp now);

public:
    Timer(std::function<void()> cb, Timestamp time, double interval);
    ~Timer() = default;

    void run() const;
    Timestamp getExpiration() const;
    bool isRepeat() const;
};

