#pragma once
#include <arpa/inet.h>
#include "common.h"

class InetAddress
{
private:
    sockaddr_in addr;
    socklen_t addr_len;
public:
    DISALLOW_COPY_AND_MOVE(InetAddress);
    InetAddress();
    InetAddress(const char* ip, uint16_t port);
    ~InetAddress();

    void setInetAddr(sockaddr_in _addr, socklen_t _addr_len);
    sockaddr_in getAddr();
    socklen_t getAddr_len();
};
class Socket
{
private:
    int fd;
public:
    DISALLOW_COPY_AND_MOVE(Socket);
    Socket();
    Socket(int _fd);
    ~Socket();

    void bind(InetAddress*);
    void listen();
    int accept(InetAddress*);
    void connect(InetAddress*);
    void connect(const char *ip, uint16_t port);
    void setnonblocking();
    int getFd();
    bool isNonBlocking();
};
