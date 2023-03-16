#include "HttpServer.h"
#include "HttpRequest.h"
#include "HttpResponse.h"
#include "HttpContext.h"
#include "Timestamp.h"

void onRequest(const HttpRequest& req, HttpResponse* resp)
{
    if (req.path() == "/")
    {
        resp->setStatusCode(HttpResponse::k200Ok);
        resp->setStatusMessage("OK");
        resp->setContentType("text/html");
        std::string now = Timestamp::now().toFormattedString();
        resp->setBody("<html><head><title>Title</title></head>"
            "<body><h1>Hello world</h1>Now is " + now +
            "</body></html>");
        resp->setCloseConnection(true);
    }
    else if (req.path() == "/hello")
    {
        resp->setStatusCode(HttpResponse::k200Ok);
        resp->setStatusMessage("OK");
        resp->setContentType("text/plain");
        resp->setBody("hello, world!\n");
        resp->setCloseConnection(true);
    }
    else
    {
        resp->setStatusCode(HttpResponse::k404NotFound);
        resp->setStatusMessage("Not Found");
        resp->setCloseConnection(true);
    }
}

int main()
{
    EventLoop loop;
    HttpServer server(&loop, "0.0.0.0", 8888);
    server.setHttpCallback(onRequest);
    server.start();
    loop.loop();
}