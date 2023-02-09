#include "Timer.h"

Timer::Timer(std::function<void()> cb, Timestamp time, double  interval)
: callback_(std::move(cb)), expiration_(time), interval_(interval), repeat_(interval > 0.0), canceled_(false){}

void Timer::run() const
{
    callback_();
}
Timestamp Timer::getExpiration() const
{
    return expiration_;
}
bool Timer::isRepeat() const
{
    return repeat_;
}
void Timer::restart(Timestamp time)
{
    expiration_ = addTime(expiration_, interval_); 
}
