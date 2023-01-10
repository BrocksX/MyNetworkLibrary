#pragma once

#include <hiredis/hiredis.h>
#include <string>
#include <vector>
#include "common.h"
class Redis
{
public:
	DISALLOW_COPY_AND_MOVE(Redis);
    Redis() = default;
    ~Redis() = default;

 	//创建连接
	bool connect(std::string ip, uint16_t port, std::string password = "");
	//get请求
    std::string get(std::string key);
	//set请求
	void set(std::string key, std::string value);
	void setWithTimeout(std::string key, std::string value, int time);

    void zadd(std::string key, std::string score, std::string member);
    bool zrange(const std::string& key, int64_t start, int64_t stop, std::vector<std::string>& values, bool withScores);
 
    void disconnect();
private:
    bool execReturnArray(const std::string& cmd, std::vector<std::string>& ret);
    std::unique_ptr<redisContext> connect_;
	std::unique_ptr<redisReply> reply_;
    //std::string ip_;
    //uint16_t port_;
	//std::string pwd_;
};
