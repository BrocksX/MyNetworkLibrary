#include "Timer.h"

Timer::Timer(std::function<void()> cb, Timestamp time, double  interval)
: callback_(std::move(cb)), expiration_(time), interval_(interval), repeat_(interval > 0.0){}

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
    if(repeat_)
        expiration_ = addTime(time, interval_);
    else
        //recheck
        expiration_ = Timestamp();
}