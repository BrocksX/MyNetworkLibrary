#include "Redis.h"

bool Redis::connect(const std::string &ip, const uint16_t &port, const std::string &password)
{
    if (context_)
    {
        redisFree(context_.get());
    }
    context_ = std::unique_ptr<redisContext>(redisConnect(ip.c_str(), port));
    if (!password.empty())
    {
        std::string cmd = "AUTH " + password;
        redisReply *reply = (redisReply *)::redisCommand(context_.get(), cmd.c_str());
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
    if (context_ && context_->err)
    {
        printf("connect error: %s\n", context_->errstr);
        return false;
    }
    return true;
}

void Redis::disconnect()
{
    if (context_.get())
    {
        redisFree(context_.get());
    }
}

bool Redis::checkAlive()
{
    std::vector<std::string> cmd = {"PING"};
    std::string ret;
    execReplyStatus(cmd, ret);
    if(ret == "PONG")   return true;
    else    return false;
}

std::string Redis::get(std::string key)
{
    redisReply *reply_ = (redisReply *)redisCommand(context_.get(), "GET %s", key.c_str());
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

bool Redis::hget(const std::string &key, const std::string &field, std::string &value)
{
    if (key.empty() || field.empty())
        return false;
    std::vector<std::string> args;
    args.push_back("HGET");
    args.push_back(key);
    args.push_back(field);
    return execReplyString(args, value);
}

bool Redis::hmget(const std::string &key, const std::vector<std::string> &fields, std::vector<std::string> &values)
{
    if (key.empty() || fields.empty())
        return false;
    std::vector<std::string> args;
    args.push_back("HMGET");
    args.push_back(key);
    for(const auto &f : fields)
    {
        args.push_back(f);
    }
    return execReplyArray(args, values);
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

int64_t Redis::zadd(std::string key, const std::unordered_map<int64_t, std::string> &score_vals)
{
    if (key.empty() || score_vals.empty())
        return false;
    std::vector<std::string> args;
    args.push_back("ZADD");
    args.push_back(key);
    for(auto const c : score_vals)
    {
        args.push_back(std::to_string(c.first));
        args.push_back(c.second);
    }
    int64_t value = 0;
    execReplyInt(args, value);
    return value;
}

bool Redis::zrange(const std::string &key, int64_t start, int64_t stop, std::vector<std::string> &values, bool withScores)
{
    if (key.empty() || start > stop)
        return false;
    std::vector<std::string> args;
    args.push_back("ZRANGE");
    args.push_back(key);
    args.push_back(std::to_string(start));
    args.push_back(std::to_string(stop));
    if(withScores) args.push_back("WITHSCORES");

    return execReplyArray(args, values);
}

bool Redis::execReplyString(const std::vector<std::string> &args, std::string &ret)
{
    if (!context_)
        return false;
    ret.clear();
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
    redisReply *reply = (redisReply *)redisCommandArgv(context_.get(), argv.size(), &(argv[0]), &(argvlen[0]));
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

bool Redis::execReplyArray(const std::vector<std::string> &args, std::vector<std::string> &ret)
{
    if (!context_)
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
    redisReply *reply = (redisReply *)redisCommandArgv(context_.get(), argv.size(), &(argv[0]), &(argvlen[0]));
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
    if (!context_)
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
    redisReply *reply = (redisReply *)redisCommandArgv(context_.get(), argv.size(), &(argv[0]), &(argvlen[0]));
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
    if (!context_)
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
    redisReply *reply = (redisReply *)redisCommandArgv(context_.get(), argv.size(), &(argv[0]), &(argvlen[0]));
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
