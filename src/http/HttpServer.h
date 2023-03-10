#pragma once

#include "TcpServer.h"
#include "noncopyable.h"
#include "InetAddress.h"
#include "Timestamp.h"
#include "Connection.h"
#include "EventLoop.h"
#include <string>

class HttpRequest;
class HttpResponse;
class InetAddress;
class HttpServer
{
public:
    DISALLOW_COPY_AND_MOVE(HttpServer);
    using HttpCallback = std::function<void (const HttpRequest&, HttpResponse*)>;

    HttpServer(EventLoop *loop, const char* ip, const uint16_t &port);

    void setHttpCallback(const HttpCallback& cb)
    {
        httpCallback_ = cb;
    }
    
    void start();

private:
    void onConnection(std::shared_ptr<Connection> conn);
    void onMessage(std::shared_ptr<Connection> conn);
    void onRequest(std::shared_ptr<Connection> conn, const HttpRequest&);

    TcpServer server_;
    HttpCallback httpCallback_;
};
