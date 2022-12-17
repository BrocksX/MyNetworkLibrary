#pragma once
#include <sys/time.h>
#include <iostream>
#include <string>
class Timestamp
{
private:
    int64_t microSecondsSinceEpoch_;
public:
    Timestamp();
    explicit Timestamp(int64_t microSecondsSinceEpoch);
    ~Timestamp();

    static Timestamp now();
    std::string toString() const;
    std::string toFormattedString(bool showMicroseconds = false) const;

    int64_t getMicroSecondsSinceEpoch() const;
    time_t getSecondsSinceEpoch() const;

    static const int kMicroSecondsPerSecond = 1000 * 1000;
};

inline bool operator<(Timestamp lhs, Timestamp rhs)
{
    return lhs.getMicroSecondsSinceEpoch() < rhs.getMicroSecondsSinceEpoch();
}

inline bool operator==(Timestamp lhs, Timestamp rhs)
{
    return lhs.getMicroSecondsSinceEpoch() == rhs.getMicroSecondsSinceEpoch();
}


inline Timestamp addTime(Timestamp timestamp, double seconds)
{
    // 将延时的秒数转换为微妙
    int64_t delta = static_cast<int64_t>(seconds * Timestamp::kMicroSecondsPerSecond);
    // 返回新增时后的时间戳
    return Timestamp(timestamp.getMicroSecondsSinceEpoch() + delta);
}