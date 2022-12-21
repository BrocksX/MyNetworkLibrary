#include "Redis.h"

Redis::~Redis()
{
    connect_ = NULL;
    reply_ = NULL;	
}

bool Redis::connect(std::string host, int port)
{
    connect_ = redisConnect(host.c_str(), port);
    if(connect_ != NULL && connect_->err)
    {
        printf("connect error: %s\n", connect_->errstr);
        return 0;
    }
    return 1;
}

std::string Redis::get(std::string key)
{
    reply_ = (redisReply*)redisCommand(connect_, "GET %s", key.c_str());
    std::string str = reply_->str;
    freeReplyObject(reply_);
    return str;
}

void Redis::set(std::string key, std::string value)
{
    redisCommand(connect_, "SET %s %s", key.c_str(), value.c_str());
}

void Redis::setWithTimeout(std::string key, std::string value, int time = 300)
{
    redisCommand(connect_, "SET %s %s  EX %d", key.c_str(), value.c_str(),time);
}