#include "RedisConnectPool.h"
#include <algorithm>

RedisConnectPool* RedisConnectPool::getConnectionPool(const std::string ip, uint16_t port, int size, const std::string passwd)
{
    static RedisConnectPool pool(ip, port, size, passwd);
    return &pool;
}

RedisConnectPool::RedisConnectPool(const std::string ip, uint16_t port, int size, const std::string passwd) : ip_(ip), port_(port), password_(passwd), size_(size)
{
    std::unique_lock<std::mutex> locker(mutex_);
    for (int i = 0; i < size_; ++i)
    {
        Redis *conn = new Redis();
        if(conn->connect(ip_, port_, password_))
        {
            connectPool_.push(conn);
        }
        else
            throw std::runtime_error("Redis Connetion create error");
    }
    std::thread checker(&RedisConnectPool::checkConnection, this);
    checker.detach();
}

void RedisConnectPool::checkConnection()
{
    while (true)
    {
        std::this_thread::sleep_for(std::chrono::microseconds(60000));
        Redis* conn = getConnection();
        if(!conn->checkAlive())
        {
            conn->disconnect();
            delete conn;
            Redis *conn = new Redis();
            if(conn->connect(ip_, port_, password_))
            {
                connectPool_.push(conn);
            }
        }
        else
        {
            releaseConnect(conn);
        }
    }
}

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

Redis *RedisConnectPool::getConnection()
{
    std::unique_lock<std::mutex> locker(mutex_);
    if (connectPool_.empty())
    {
        while (connectPool_.empty())
        {
            // 如果为空，需要阻塞一段时间，等待新的可用连接
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
