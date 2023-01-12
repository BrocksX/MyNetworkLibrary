#include "TcpServer.h"
#include <iostream>
#include "Connection.h"
#include "Socket.h"
#include "Timestamp.h"
#include <string>
#include "json.hpp"
#include "RedisConnectPool.h"

using json = nlohmann::json_abi_v3_11_2::json;
int main()
{
    TcpServer *server = new TcpServer("0.0.0.0", 8888);
    RedisConnectPool *redisConns = new RedisConnectPool("127.0.0.1",6379, 20);
    Timestamp *clock = new Timestamp();
    std::unordered_map<int, Connection *> client;
    redisConns->connect();

    server->setOnConnectCallback([&client](Connection *conn)
                                    { client[conn->getSocket()->getFd()] = conn; });

    server->setOnRecvCallback([&redisConns, &clock, &client](Connection *conn)
                                {
    conn->read();
    if (conn->getState() == Connection::State::Closed) 
    {
        client.erase(conn->getSocket()->getFd());
        conn->close();
        return;
    }

    json receiveMsg = json::parse(conn->readBuffer());
    if(receiveMsg["sign"]=="message")//接受信息
    {
        std::string msg = receiveMsg["time"];
        msg += " - ";
        msg += receiveMsg["name"];
        msg += ": ";
        msg += receiveMsg["content"];

        //printf("%s", msg.c_str());
        std::cout << msg << std::endl;
        for(auto & cli : client)
        {
        if(cli.second != conn)
            cli.second->send(receiveMsg.dump());
        }
        std::string score = clock->now().toString(true);
        std::string member = receiveMsg.dump();
        
        Redis* red = redisConns->getConnect();
        red->zadd("message", score, member);
        redisConns->releaseConnect(red);
    }

    else if(receiveMsg["sign"]=="signup")//新用户连接
    {
        json msg;
        msg["name"] = "Server";
        msg["sign"] = "message";
        msg["time"] = clock->now().toFormattedString();
        std::string content = receiveMsg["name"];
        content += " has joint";
        msg["content"] = content;
        std::cout << content << std::endl;
        //printf("%s", content.c_str());

        std::vector<std::string> values;
        Redis* red = redisConns->getConnect();
        bool foundHistroy = red->zrange("message", 0, 1000000, values, false);
        redisConns->releaseConnect(red);
        if(foundHistroy)
        {
            for(int i = 0; i < values.size(); i++)
            {
                std::cout << values[i] << std::endl;
                conn->send(values[i]);
            }
        }
        for(auto & cli : client)
        {
            if(cli.second != conn)
                cli.second->send(msg.dump());
        }
    } });
    server->start();

    delete server;
    return 0;
}