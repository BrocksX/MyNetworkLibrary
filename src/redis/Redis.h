#pragma once

#include <hiredis/hiredis.h>
#include <string>
#include <vector>
#include <string.h>
#include "common.h"
class Redis
{
public:
	DISALLOW_COPY_AND_MOVE(Redis);
    Redis() = default;
    ~Redis() = default;

 	//创建连接
	bool connect(std::string ip, uint16_t port, std::string password = "");
    void disconnect();
    void freeReply(redisReply * reply);

	//get请求
    std::string get(std::string key);
	//set请求
	bool set(const std::string& key, const std::string& value);
	bool setWithTimeout(std::string key, std::string value, int time = 100);

    void zadd(std::string key, std::string score, std::string member);
    bool zrange(const std::string& key, int64_t start, int64_t stop, std::vector<std::string>& values, bool withScores);
 
    
private:
    std::unique_ptr<redisContext> connect_;
    bool execReturnArray(const std::string& cmd, std::vector<std::string>& ret);
    //bool execReturnStatus(const char *format, ...);
    //std::string ip_;
    //uint16_t port_;
	//std::string pwd_;
};
