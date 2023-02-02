#include "RedisConnectPool.h"
#include <algorithm>

RedisConnectPool::RedisConnectPool(const std::string ip, uint16_t port, int size, const std::string passwd) : ip_(ip), port_(port), password_(passwd), size_(size)
{}

RedisConnectPool::~RedisConnectPool()
{
    std::unique_lock<std::mutex> lock(mutex_);
    while(!connectPool_.empty())
    {
        Redis *conn = connectPool_.front();
        conn->disconnect();
        connectPool_.pop();
    }
}

bool RedisConnectPool::connect()
{
    for (int i = 0; i < size_; ++i)
    {
        Redis *conn = new Redis();
        if(conn->connect(ip_, port_, password_))
        {
            connectPool_.push(conn);
        }
        else
            return false;
    }
    return true;
}

Redis *RedisConnectPool::getConnect()
{
    std::unique_lock<std::mutex> locker(mutex_);
    if (connectPool_.empty())
    {
        while (connectPool_.empty())
        {
            // 如果为空，需要阻塞3s，等待新的可用连接
            if (std::cv_status::timeout == cv_.wait_for(locker, std::chrono::milliseconds(3000)))
            {
                if (connectPool_.empty())
                {
                    continue;
                }
            }
        }
    }
    Redis *conn = connectPool_.front();
    connectPool_.pop();
    return conn;
}

void RedisConnectPool::releaseConnect(Redis *conn)
{
    if(conn == nullptr)
        return;
    {
        std::unique_lock<std::mutex> lock(mutex_);
        connectPool_.push(conn);
        cv_.notify_all();
    }
}
