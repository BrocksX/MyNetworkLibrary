#include "Redis.h"

bool Redis::connect(const std::string &ip, const uint16_t &port, const std::string &password)
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
    if (connect_.get())
    {
        redisFree(connect_.get());
    }
}

std::string Redis::get(std::string key)
{
    redisReply *reply_ = (redisReply *)redisCommand(connect_.get(), "GET %s", key.c_str());
    printf("%d", reply_->type);
    std::string str = reply_->str;
    freeReplyObject(reply_);
    return str;
}

bool Redis::set(const std::string &key, const std::string &value)
{
    if (key.empty() || value.empty())
        return false;
    std::vector<std::string> args;
    args.emplace_back("SET");
    args.emplace_back(key);
    args.emplace_back(value);
    std::string ret;
    if(execReplyStatus(args,ret))
    {
        if(strcasecmp(ret.c_str(), "OK") == 0)
            return true;
    }
    return false;
}

bool Redis::setWithTimeout(const std::string &key, const std::string &value, const int64_t &sceonds)
{
    if (key.empty() || value.empty())
        return false;
    if (sceonds <= 0)
        return true;
    std::vector<std::string> args;
    args.emplace_back("SET");
    args.emplace_back(key);
    args.emplace_back(value);
    args.emplace_back("EX");
    args.emplace_back(std::to_string(sceonds));
    std::string ret;
    if(execReplyStatus(args,ret))
    {
        if(strcasecmp(ret.c_str(), "OK") == 0)
            return true;
    }
    return false;
}

int64_t Redis::hset(const std::string &key, const std::unordered_map<std::string, std::string> &fields_vals)
{
    if (fields_vals.empty())
        return 0;
    if (key.empty())
        return -1;
    std::vector<std::string> args;
    args.push_back("HSET");
    args.push_back(key);
    for (const auto i : fields_vals)
    {
        args.push_back(i.first);
        args.push_back(i.second);
    }
    int64_t ret = 0;
    bool ok = execReplyInt(args, ret);
    if (ok)
        return ret;
    else
        return -1;
}

bool Redis::hget(const std::string &key, const std::string &fields, std::string &values)
{
    if (key.empty() || fields.empty())
        return false;
    values.clear();
    std::vector<std::string> args;
    args.push_back("HGET");
    args.push_back(key);
    args.push_back(fields);
    return execReplyString(args, values);
}

bool Redis::hexists(const std::string &key, const std::string &field)
{
    if (key.empty() || field.empty())
        return false;
    std::vector<std::string> args;
    args.push_back("HEXISTS");
    args.push_back(key);
    args.push_back(field);
    int64_t ret = 0;
    execReplyInt(args, ret);
    if(ret == 1)
        return true;
    else
        return false;
}

void Redis::zadd(std::string key, std::string score, std::string member)
{
    redisCommand(connect_.get(), "ZADD %s %s %s", key.c_str(), score.c_str(), member.c_str());
}

bool Redis::zrange(const std::string &key, int64_t start, int64_t stop, std::vector<std::string> &values, bool withScores)
{
    std::string cmd = "ZRANGE " + key + " " + std::to_string(start) + " " + std::to_string(stop) + (withScores ? " WITHSCORES" : "");
    values.clear();
    if (execReplyArray(cmd, values))
    {
        return true;
    }
    return false;
}

bool Redis::execReplyString(const std::vector<std::string> &args, std::string &ret)
{
    if (!connect_)
        return false;
    ret.clear();
    if (args.empty())
    {
        return true;
    }
    std::vector<const char *> argv(args.size());
    std::vector<size_t> argvlen(args.size());
    int64_t j = 0;
    for (const auto &i : args)
    {
        argv[j] = i.c_str();
        argvlen[j] = i.length();
        j++;
    }
    redisReply *reply = (redisReply *)redisCommandArgv(connect_.get(), argv.size(), &(argv[0]), &(argvlen[0]));
    if (reply)
    {
        if (reply->type == REDIS_REPLY_STRING)
        {
            ret = reply->str;
            freeReplyObject(reply);
            return true;
        }
        else
        {
            freeReplyObject(reply);
            return false;
        }
    }
    return false;
}

bool Redis::execReplyArray(const std::string &cmd, std::vector<std::string> &ret)
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

bool Redis::execReplyInt(const std::vector<std::string> &args, int64_t &ret)
{
    if (!connect_)
        return false;
    if (args.empty())
    {
        ret = 0;
        return true;
    }
    std::vector<const char *> argv(args.size());
    std::vector<size_t> argvlen(args.size());
    int64_t j = 0;
    for (const auto &i : args)
    {
        argv[j] = i.c_str();
        argvlen[j] = i.length();
        j++;
    }
    redisReply *reply = (redisReply *)redisCommandArgv(connect_.get(), argv.size(), &(argv[0]), &(argvlen[0]));
    if (reply)
    {
        if (reply->type == REDIS_REPLY_INTEGER)
        {
            ret = reply->integer;
            freeReplyObject(reply);
            return true;
        }
        else
        {
            freeReplyObject(reply);
            return false;
        }
    }
    return false;
}

bool Redis::execReplyStatus(const std::vector<std::string> &args, std::string &ret)
{
    if (!connect_)
        return false;
    if (args.empty())
        return true;

    std::vector<const char *> argv(args.size());
    std::vector<size_t> argvlen(args.size());
    int64_t j = 0;
    for (const auto &i : args)
    {
        argv[j] = i.c_str();
        argvlen[j] = i.length();
        j++;
    }
    redisReply *reply = (redisReply *)redisCommandArgv(connect_.get(), argv.size(), &(argv[0]), &(argvlen[0]));
    if (reply)
    {
        if(reply->type == REDIS_REPLY_STATUS)
        {
            ret = reply->str;
            freeReplyObject(reply);
            return true;
        }
        else
        {
            freeReplyObject(reply);
            return false;
        }
        
    }
    return false;
}
