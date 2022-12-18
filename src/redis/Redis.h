#pragma once

#include <hiredis/hiredis.h>
#include <string>
class Redis
{
public:
 
    Redis(){}
 //释放资源
    ~Redis()
	{
        this->_connect = NULL;
		this->_reply = NULL;	    	    
	}
 //创建连接
	bool connect(std::string host, int port)
	{
	    this->_connect = redisConnect(host.c_str(), port);
		if(this->_connect != NULL && this->_connect->err)
		{
		    printf("connect error: %s\n", this->_connect->errstr);
			return 0;
		}
		return 1;
	}
 //get请求
    std::string get(std::string key)
	{
		this->_reply = (redisReply*)redisCommand(this->_connect, "GET %s", key.c_str());
		std::string str = this->_reply->str;
		freeReplyObject(this->_reply);
		return str;
	}
//set请求
	void set(std::string key, std::string value)
	{
        redisCommand(this->_connect, "SET %s %s", key.c_str(), value.c_str());
	}
 
private:
 
    redisContext* _connect;
	redisReply* _reply;
				
};
