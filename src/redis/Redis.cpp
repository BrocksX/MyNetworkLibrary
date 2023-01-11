#include "Redis.h"


bool Redis::connect(std::string ip, uint16_t port, std::string password)
{
    if (connect_)
    {
        redisFree(connect_.get());
    }
    connect_ = std::unique_ptr<redisContext>(redisConnect(ip.c_str(), port));
    if (!password.empty())
    {
        std::string cmd = "AUTH " + password;
        redisReply *reply = (redisReply *)::redisCommand(connect_.get(), cmd.c_str());
        if (reply == NULL)
        {
            return false;
        }
        else if (reply->str != "OK")
        {
            freeReplyObject(reply);
            return false;
        }
    }
    if (connect_ && connect_->err)
    {
        printf("connect error: %s\n", connect_->errstr);
        return false;
    }
    return true;
}

void Redis::disconnect()
{
    if (connect_.get() != nullptr)
    {
        redisFree(connect_.get());
    }
}

std::string Redis::get(std::string key)
{
    redisReply *reply_ = (redisReply *)redisCommand(connect_.get(), "GET %s", key.c_str());
    std::string str = reply_->str;
    freeReplyObject(reply_);
    return str;
}

bool Redis::set(const std::string &key, const std::string &value)
{
    if (connect_ == NULL)
    {
        return false;
    }
    redisReply *reply = (redisReply *)redisCommand(connect_.get(), "SET %s %s ", key.c_str(), value.c_str());
    if (reply != nullptr)
    {
        if (strcasecmp(reply->str, "OK") == 0)
        {
            return true;
        }
        freeReply(reply);
    }
    return true;
}

bool Redis::setWithTimeout(std::string key, std::string value, int time)
{
    if (connect_ == NULL)
    {
        return false;
    }
    redisReply *reply = (redisReply *)redisCommand(connect_.get(), "SET %s %s  EX %d", key.c_str(), value.c_str(), time);
    if (reply != nullptr)
    {
        if (strcasecmp(reply->str, "OK") == 0)
        {
            return true;
        }
        freeReply(reply);
    }
    return true;
    redisCommand(connect_.get(), "SET %s %s  EX %d", key.c_str(), value.c_str(), time);
}

void Redis::zadd(std::string key, std::string score, std::string member)
{
    redisCommand(connect_.get(), "ZADD %s %s %s", key.c_str(), score.c_str(), member.c_str());
}

bool Redis::execReturnArray(const std::string &cmd, std::vector<std::string> &ret)
{
    if (!connect_)
    {
        return false;
    }

    redisReply *reply = (redisReply *)::redisCommand(connect_.get(), cmd.c_str());
    if (reply != NULL)
    {
        if (reply->type == REDIS_REPLY_ARRAY)
        {
            int32_t elements = reply->elements;
            ret.reserve(elements);
            for (int32_t i = 0; i < elements; ++i)
            {
                std::string strTemp(reply->element[i]->str, reply->element[i]->len);
                ret.push_back(strTemp);
            }
            freeReplyObject(reply);
            return true;
        }
    }
    freeReplyObject(reply);
    return false;
}

bool Redis::zrange(const std::string &key, int64_t start, int64_t stop, std::vector<std::string> &values, bool withScores)
{
    std::string cmd = "ZRANGE " + key + " " + std::to_string(start) + " " + std::to_string(stop) + (withScores ? " WITHSCORES" : "");
    values.clear();
    if (execReturnArray(cmd, values))
    {
        return true;
    }
    return false;
}

void Redis::freeReply(redisReply *reply)
{
    if (reply)
    {
        freeReplyObject(reply);
    }
}
