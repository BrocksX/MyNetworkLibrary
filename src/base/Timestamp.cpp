#include "Timestamp.h"

Timestamp::Timestamp(int64_t microSecondsSinceEpoch): microSecondsSinceEpoch_(microSecondsSinceEpoch)
{
}
Timestamp::Timestamp(): microSecondsSinceEpoch_(0)
{
}
Timestamp::~Timestamp()
{
}
time_t Timestamp::getSecondsSinceEpoch() const
{ 
    return static_cast<time_t>(microSecondsSinceEpoch_ / kMicroSecondsPerSecond); 
}

int64_t Timestamp::getMicroSecondsSinceEpoch() const 
{ 
    return microSecondsSinceEpoch_; 
}

Timestamp Timestamp::now()
{
    struct timeval tv;
    gettimeofday(&tv, NULL);
    int64_t seconds = tv.tv_sec;
    // 转换为微妙
    return Timestamp(seconds * kMicroSecondsPerSecond + tv.tv_usec);
}

std::string Timestamp::toFormattedString(bool showMicroseconds) const
{
    char buf[64] = {0};
    time_t seconds = static_cast<time_t>(microSecondsSinceEpoch_ / kMicroSecondsPerSecond);
    // 使用localtime函数将秒数格式化成日历时间
    tm *tm_time = localtime(&seconds);
    if (showMicroseconds)
    {
        int microseconds = static_cast<int>(microSecondsSinceEpoch_ % kMicroSecondsPerSecond);
        snprintf(buf, sizeof(buf), "%4d/%02d/%02d %02d:%02d:%02d.%06d",
                tm_time->tm_year + 1900,
                tm_time->tm_mon + 1,
                tm_time->tm_mday,
                tm_time->tm_hour,
                tm_time->tm_min,
                tm_time->tm_sec,
                microseconds);
    }
    else
    {
        snprintf(buf, sizeof(buf), "%4d/%02d/%02d %02d:%02d:%02d",
                tm_time->tm_year + 1900,
                tm_time->tm_mon + 1,
                tm_time->tm_mday,
                tm_time->tm_hour,
                tm_time->tm_min,
                tm_time->tm_sec);
    }
    return buf;
}
