#include "TcpServer.h"
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

    printf("%s\n", conn->readBuffer());
    conn->send(conn->readBuffer()); 
    });

    server->start();
    delete server;
    return 0;
}