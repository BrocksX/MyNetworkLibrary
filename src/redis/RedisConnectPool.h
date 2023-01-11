#pragma once
#include <vector>
#include <list>
#include <mutex>
#include "Redis.h"
#include "common.h"

class RedisConnectPool
{
private:
    int size_;
    int usedCount_;
    std::string ip_;
    std::string password_;
    uint16_t port_;
    std::list<Redis *> connectPool_;
    std::mutex mutex_;

public:
    DISALLOW_COPY_AND_MOVE(RedisConnectPool);
    RedisConnectPool(const std::string ip, uint16_t port, int size, const std::string passwd = "");
    ~RedisConnectPool();

    bool connect();
    Redis* getConnect();
    void releaseConnect(Redis *conn);
    //bool isUseful(Redis *conn);
    int getFreeNum();
    int getUsedCount();
};
