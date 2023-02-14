#include "MysqlconntectPool.h"
#include <fstream>
#include <thread>
#include <assert.h>

MysqlConnectionPool::MysqlConnectionPool(const std::string &ip, const uint16_t &port, const std::string &username, const std::string &password, 
        const std::string &dbName, const int &size, const int &timeout, const int &maxIdletime) : ip_(ip), port_(port), 
        user_(username), passwd_(password), dbName_(dbName), size_(size), timeout_(timeout), maxIdleTime_(maxIdletime)
{
    for (int i = 0; i < size_; ++i)
    {
        addConnection();
        currentSize_++;
    }
}

MysqlConnectionPool::~MysqlConnectionPool()
{
    while (!connectPool_.empty())
    {
        Mysql* conn = connectPool_.front();
        connectPool_.pop();
        delete conn;
        currentSize_--;
    }
}



void MysqlConnectionPool::addConnection()
{
    Mysql* conn = new Mysql;
    conn->connect(user_, passwd_, dbName_, ip_, port_);
    conn->refreshAliveTime();    // 刷新起始的空闲时间点
    connectPool_.push(conn); // 记录新连接
    currentSize_++;
}

// 获取连接
std::shared_ptr<Mysql> MysqlConnectionPool::getConnection()
{
    std::unique_lock<std::mutex> locker(mutex_);
    if (connectPool_.empty())
    {
        while (connectPool_.empty())
        {
            // 如果为空，需要阻塞一段时间，等待新的可用连接
            if (std::cv_status::timeout == cv_.wait_for(locker, std::chrono::milliseconds(timeout_)))
            {
                // std::cv_status::timeout 表示超时
                if (connectPool_.empty())
                {
                    continue;
                }
            }
        }
    }
    
    // 使用共享智能指针并规定其删除器
    std::shared_ptr<Mysql> connptr(connectPool_.front(), 
        [this](Mysql* conn) {
            std::lock_guard<std::mutex> locker(mutex_);
            conn->refreshAliveTime();
            connectPool_.push(conn);
        });
    connectPool_.pop();
    cv_.notify_all();
    return connptr;
}