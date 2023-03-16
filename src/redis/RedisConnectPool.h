#pragma once
#include <vector>
#include <queue>
#include <mutex>
#include <thread>
#include <condition_variable>
#include "Redis.h"
#include "noncopyable.h"

class RedisConnectPool
{
public:
    DISALLOW_COPY_AND_ASSIGN(RedisConnectPool);
    static RedisConnectPool* getConnectionPool(const std::string ip, uint16_t port, int size, const std::string passwd = "");
    ~RedisConnectPool();
    Redis* getConnection();
    void releaseConnect(Redis *conn);

private:
    RedisConnectPool(const std::string ip, uint16_t port, int size, const std::string passwd = "");
    void checkConnection();
    int size_;
    std::string ip_;
    std::string password_;
    uint16_t port_;
    std::queue<Redis *> connectPool_;
    std::mutex mutex_;
    std::condition_variable cv_;
};
