#include "HttpServer.h"
#include "HttpRequest.h"
#include "HttpResponse.h"
#include "HttpContext.h"

#include <memory>

/**
 * 默认的http回调函数
 * 设置响应状态码，响应信息并关闭连接
 */
void defaultHttpCallback(const HttpRequest&, HttpResponse* resp)
{
    resp->setStatusCode(HttpResponse::k404NotFound);
    resp->setStatusMessage("Not Found");
    resp->setCloseConnection(true);
}

HttpServer::HttpServer(EventLoop *loop, const char* ip, const uint16_t &port) : server_(loop, ip, port), httpCallback_(defaultHttpCallback)
{
    server_.setMessageCallback(std::bind(&HttpServer::onMessage, this, std::placeholders::_1));
}

void HttpServer::start()
{
    server_.start();
}

void HttpServer::onConnection(Connection* conn)
{
    
}

void HttpServer::onMessage(Connection* conn)
{
    std::unique_ptr<HttpContext> context(new HttpContext);
    conn->read();
    // 进行状态机解析
    if (!context->parseRequest(conn->getReadBuffer(), Timestamp::now()))
    {
        conn->send("HTTP/1.1 400 Bad Request\r\n\r\n");
        conn->close();
    }
    if (context->gotAll())
    {
        onRequest(conn, context->request());
        context->reset();
    }
}

void HttpServer::onRequest(Connection* conn, const HttpRequest& req)
{
    const std::string& connection = req.getHeader("Connection");

    // 判断长连接还是短连接
    bool close = connection == "close" || (req.version() == HttpRequest::kHttp10 && connection != "Keep-Alive");
    HttpResponse response(close);
    httpCallback_(req, &response);
    Buffer buf;
    response.appendToBuffer(&buf);
    conn->send(buf);
    if (response.closeConnection())
    {
        conn->close();
    }
}