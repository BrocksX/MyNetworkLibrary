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
    EventLoop *loop = new EventLoop();
    TcpServer *server = new TcpServer(loop, "0.0.0.0", 8888);

    server->setMessageCallback([](Connection *conn)
                              {
    conn->read();
    if (conn->getState() == Connection::State::Closed) {
      conn->close();
      return;
    }

    std::cout<<conn->readBuffer()<<std::endl;
    conn->setSendBuffer(conn->readBuffer());
    conn->write(); });

    server->start();
    delete server;
    return 0;
}