#include "TcpServer.h"
#include <iostream>
#include "Buffer.h"
#include "Connection.h"
#include "EventLoop.h"
#include "Socket.h"

int main() {
  TcpServer *server = new TcpServer("127.0.0.1", 8888);
  server->setOnConnectCallback([](Connection *conn) { std::cout << "New connection fd: " << conn->getSocket()->getFd() << std::endl; });
  server->setOnRecvCallback([](Connection *conn) {
    conn->read();
    if (conn->getState() == Connection::State::Closed) {
      conn->close();
      return;
    }
    std::cout << "Message from client " << conn->getSocket()->getFd() << ": " << conn->readBuffer() << std::endl;
    conn->setSendBuffer(conn->readBuffer());
    conn->write();
  });
  server->start();
  
  delete server;
  return 0;
}