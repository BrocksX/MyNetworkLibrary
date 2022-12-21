#include <Connection.h>
#include <Socket.h>
#include <iostream>

int main()
{
    Socket *sock = new Socket();
    sock->connect("127.0.0.1", 8888);

    Connection *conn = new Connection(nullptr, sock);
    while (true)
    {
        conn->read();
        std::cout << "Message from server: " << conn->readBuffer() << std::endl;
    }
    conn->read();

    if (conn->getState() == Connection::State::Connected)
    {
        std::cout << conn->readBuffer() << std::endl;
    }
    conn->setSendBuffer("Hello server!");
    conn->write();
    return 0;
}