#include <Connection.h>
#include <Socket.h>
#include <iostream>
#include "ThreadPool.h"
#include "json.hpp"
#include "Timestamp.h"

using json = nlohmann::json_abi_v3_11_2::json;

void recMsg(Connection *conn)
{
    while (true)
    {
        conn->read();
        std::string msg = conn->readBuffer();
        size_t index = 0;
        size_t last = msg.find('}', index);
        while (last != std::string::npos)
        {
            std::string subMsg = msg.substr(index, last + 1 - index);
            json receiveMsg = json::parse(subMsg);
            std::string text = receiveMsg["time"];
            text += " - ";
            text += receiveMsg["name"];
            text += ": ";
            text += receiveMsg["content"];
            std::cout << text << std::endl;
            index = last + 1;
            last = msg.find('}', index);
        }
    }
}

int main()
{
    Timestamp *clock = new Timestamp();
    Socket *sock = new Socket();
    sock->connect("127.0.0.1", 8888);
    ThreadPool *threadPool = new ThreadPool(1);
    Connection *conn = new Connection(nullptr, sock);

    std::function<void()> read = std::bind(recMsg, conn);
    threadPool->add(std::move(read));

    std::cout << "Please input your name." << std::endl;
    std::string id = "";
    if (std::getline(std::cin, id))
    {
        std::cout << "Hello " << id << std::endl;
        json msg;
        msg["name"] = id;
        msg["content"] = "";
        msg["sign"] = "signup";
        msg["time"] = clock->now().toFormattedString();
        conn->send(msg.dump().c_str());
    }
    std::string line = "";
    while (std::getline(std::cin, line))
    {
        json msg;
        msg["name"] = id;
        msg["content"] = line;
        msg["sign"] = "message";
        msg["time"] = clock->now().toFormattedString();
        conn->send(msg.dump().c_str());
    }

    delete sock;
    delete threadPool;
    delete conn;
    delete clock;
    return 0;
}
