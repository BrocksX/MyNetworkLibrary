#include "RedisConnectPool.h"
#include <algorithm>

RedisConnectPool::RedisConnectPool(const std::string ip, uint16_t port, int size, const std::string passwd) : ip_(ip), port_(port), password_(passwd), size_(size)
{
}
RedisConnectPool::~RedisConnectPool()
{
    {
        std::unique_lock<std::mutex> lock(mutex_);
        for (std::list<Redis*>::iterator it = connectPool_.begin();
             it != connectPool_.end(); ++it)
        {
            (*it)->disconnect();
        }
        connectPool_.clear();
    }
}

bool RedisConnectPool::connect()
{
    for (int i = 0; i < size_; ++i)
    {
        Redis *conn = new Redis();
        if(conn->connect(ip_, port_, password_))
        {
            connectPool_.push_back(conn);
        }
        else
            return false;
    }
    return true;
}

Redis *RedisConnectPool::getConnect()
{
    Redis *conn = nullptr;
    {
        std::unique_lock<std::mutex> lock(mutex_);
        if(connectPool_.empty())
        {
            return nullptr;
        }
        else
        {
            conn = connectPool_.front();
            connectPool_.pop_front();
            usedCount_++;
        }
    }
    if(conn == nullptr)
    {
        conn = new Redis();
        if(!conn->connect(ip_, port_, password_))
        {
            exit(0);
        }
    }
    return conn;
}

void RedisConnectPool::releaseConnect(Redis *conn)
{
    if(conn == nullptr)
        return;
    if(find(connectPool_.begin(), connectPool_.end(), conn) != connectPool_.end())
        return;
    {
        std::unique_lock<std::mutex> lock(mutex_);
        connectPool_.push_back(conn);
        usedCount_--;
    }
}

int RedisConnectPool::getFreeNum(){return connectPool_.size();}
int RedisConnectPool::getUsedCount(){return usedCount_;}