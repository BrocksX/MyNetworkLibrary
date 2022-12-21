#pragma once

#include <hiredis/hiredis.h>
#include <string>
#include "common.h"
class Redis
{
public:
	DISALLOW_COPY_AND_MOVE(Redis);
    Redis() = default;
    ~Redis();

 	//创建连接
	bool connect(std::string host, int port);
	//get请求
    std::string get(std::string key);
	//set请求
	void set(std::string key, std::string value);
	void setWithTimeout(std::string key, std::string value, int time);
 
private:
    redisContext* connect_;
	redisReply* reply_;
				
};
