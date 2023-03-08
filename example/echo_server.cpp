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

    server->setMessageCallback([](std::shared_ptr<Connection> conn)
                              {
        conn->read();
        if (conn->getState() == Connection::State::Closed) {
            conn->close();
            return;
        }

        conn->send(conn->readBuffer());
    });

    server->start();
    delete server;
    return 0;
}