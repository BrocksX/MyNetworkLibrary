#include "Mysql.h"

Mysql::Mysql()
{
    conn_ = mysql_init(nullptr);
    mysql_set_character_set(conn_, "utf8"); 
}

Mysql::~Mysql()
{
    if (conn_ != nullptr) {
        mysql_close(conn_);
    }
    // 释放结果集
    freeResult();
}

bool Mysql::connect(const std::string& user, const std::string& passwd, const std::string dbName, const std::string& ip, const unsigned int& port)
{
    MYSQL* ptr = mysql_real_connect(conn_, ip.c_str(), user.c_str(), passwd.c_str(), dbName.c_str(), port, nullptr, 0);
    return ptr != nullptr;
}

bool Mysql::update(const std::string& sql)
{
    if (mysql_query(conn_, sql.c_str()))
    {
        return false;
    }
    return true;
}

bool Mysql::query(const std::string& sql)
{
    // 查询前确保结果集为空
    freeResult();
    if (mysql_query(conn_, sql.c_str()))
    {
        return false;
    }
    result_ = mysql_store_result(conn_);
    return true;
}

bool Mysql::next()
{
    if (result_ != nullptr)
    {
        row_ = mysql_fetch_row(result_);
        if (row_ != nullptr)
        {
            return true;
        }
    }
    return false;
}

std::string Mysql::value(int index)
{
    int rowCount = mysql_num_fields(result_);
    if (index >= rowCount || index < 0)
    {
        // 获取字段索引不合法，返回空字符串
        return std::string();
    }
    // 考虑到储存的可能是二进制字符串，其中含有'\0'
    char* val = row_[index];
    unsigned long length = mysql_fetch_lengths(result_)[index];
    return std::string(val, length);
}

bool Mysql::transaction()
{
    // true  自动提交
    // false 手动提交
    return mysql_autocommit(conn_, false);
}

bool Mysql::commit()
{
    return mysql_commit(conn_);
}

bool Mysql::rollbock()
{
    return mysql_rollback(conn_);
}

void Mysql::refreshAliveTime()
{
    m_alivetime = std::chrono::steady_clock::now();
}

long long Mysql::getAliveTime()
{
    std::chrono::nanoseconds res = std::chrono::steady_clock::now() - m_alivetime;
    std::chrono::milliseconds millsec = std::chrono::duration_cast<std::chrono::milliseconds>(res);
    return millsec.count();
}

void Mysql::freeResult()
{
    if (result_)
    {
        mysql_free_result(result_);
        result_ = nullptr;
    }
}