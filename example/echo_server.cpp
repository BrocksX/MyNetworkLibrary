#include "TcpServer.h"
#include <iostream>
#include "Buffer.h"
#include "Connection.h"
#include "Socket.h"
#include "RedisConnectPool.h"
#include "Timestamp.h"
#include "EventLoop.h"

int main()
{
    RedisConnectPool* redisConns = RedisConnectPool::getConnectionPool("127.0.0.1", 6379, 20);
    Timestamp *clock = new Timestamp();
    EventLoop *loop = new EventLoop();

    TcpServer *server = new TcpServer(loop, "0.0.0.0", 8888);

    server->setMessageCallback([&redisConns, &clock](Connection *conn)
                              {
    conn->read();
    if (conn->getState() == Connection::State::Closed) {
      conn->close();
      return;
    }

    Redis* red = redisConns->getConnect();
    red->setWithTimeout(clock->now().toString(true), conn->readBuffer());
    redisConns->releaseConnect(red);
    
    std::cout<<conn->readBuffer()<<std::endl;
    conn->setSendBuffer(conn->readBuffer());
    conn->write(); });

    server->start();
    delete server;
    return 0;
}