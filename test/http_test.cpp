#include "HttpServer.h"

int main()
{
    HttpServer server("127.0.0.1", 8888);
    server.start();
    return 0;
}