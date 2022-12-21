#include "TcpServer.h"
#include <iostream>
#include "Connection.h"
#include "Socket.h"
#include "Redis.h"
#include "Timestamp.h"
#include <string>
int main()
{
  TcpServer *server = new TcpServer("127.0.0.1", 8888);
  Redis *redis = new Redis();
  Timestamp *clock = new Timestamp();
  std::unordered_map<int, Connection*> client;


  redis->connect("127.0.0.1", 6379);
  server->setOnConnectCallback([&client](Connection *conn) 
  { 
    std::cout << "New connection fd: " << conn->getSocket()->getFd() << std::endl; 
    client[conn->getSocket()->getFd()] = conn;
  });


  server->setOnRecvCallback([&redis,&clock, &client](Connection *conn)
  {
    conn->read();
    if (conn->getState() == Connection::State::Closed) {
      conn->close();
      return;
    }
    std::cout << "Message from client " << conn->getSocket()->getFd() << ": " << conn->readBuffer() << std::endl;
    
    std::string key = clock->now().toFormattedString(true) + "- client : ";
    key += std::to_string(conn->getSocket()->getFd());
    //堆溢出
    redis->setWithTimeout(key, conn->readBuffer(), 100);
  
    for(auto & cli : client)
    {
      cli.second->send(conn->readBuffer());
    }
  });
  server->start();

  delete server;
  return 0;
}