#include "TcpServer.h"
#include <iostream>
#include "Buffer.h"
#include "Connection.h"
#include "EventLoop.h"
#include "Socket.h"

int main() {
  EventLoop *loop = new EventLoop();
  Server *server = new Server(loop);
  server->OnConnect([](Connection *conn) {
    conn->read();
    if (conn->getState() == Connection::State::Closed) {
      conn->close();
      return;
    }
    std::cout << "Message from client " << conn->getSocket()->getFd() << ": " << conn->readBuffer() << std::endl;
    conn->setSendBuffer(conn->readBuffer());
    conn->write();
  });

  loop->loop();
  delete server;
  delete loop;
  return 0;
}