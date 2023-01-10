#include "TcpServer.h"
#include <iostream>
#include "Buffer.h"
#include "Connection.h"
#include "Socket.h"
#include "RedisConnectPool.h"
#include "Timestamp.h"

int main()
{
    RedisConnectPool *redisConns = new RedisConnectPool("127.0.0.1",6379, 20);
    redisConns->connect();
    Timestamp *clock = new Timestamp();


    TcpServer *server = new TcpServer("127.0.0.1", 8888);
    server->setOnConnectCallback([](Connection *conn)
                                 { std::cout << "New connection fd: " << conn->getSocket()->getFd() << std::endl; });


    server->setOnRecvCallback([&redisConns, &clock](Connection *conn)
                              {
    conn->read();
    if (conn->getState() == Connection::State::Closed) {
      conn->close();
      return;
    }

    Redis* red = redisConns->getConnect();
    red->set(clock->now().toFormattedString(true), conn->readBuffer());
    redisConns->releaseConnect(red);
    //delete red;

    std::cout << "Message from client " << conn->getSocket()->getFd() << ": " << conn->readBuffer() << std::endl;
    conn->setSendBuffer(conn->readBuffer());
    conn->write(); });
    server->start();
    
    delete server;
    return 0;
}