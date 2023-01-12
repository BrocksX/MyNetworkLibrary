#include "TcpServer.h"
#include <iostream>
#include "Buffer.h"
#include "Connection.h"
#include "Socket.h"
#include "RedisConnectPool.h"
#include "Timestamp.h"

int main()
{
    RedisConnectPool *redisConns = new RedisConnectPool("127.0.0.1", 6379, 20);
    redisConns->connect();
    Timestamp *clock = new Timestamp();

    TcpServer *server = new TcpServer("0.0.0.0", 8888);

    server->setOnRecvCallback([&redisConns, &clock](Connection *conn)
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