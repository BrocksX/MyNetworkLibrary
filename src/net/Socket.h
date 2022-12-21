#pragma once
#include <arpa/inet.h>
#include "common.h"

class InetAddress
{
public:
    InetAddress();
    InetAddress(const char *ip, uint16_t port);
    ~InetAddress() = default;

    DISALLOW_COPY_AND_MOVE(InetAddress);

    void setAddr(sockaddr_in addr);
    sockaddr_in getAddr();
    const char *getIp();
    uint16_t getPort();

private:
    sockaddr_in addr_{};
};

class Socket
{
private:
    int fd_{-1};

public:
    Socket();
    explicit Socket(int fd);
    ~Socket();

    DISALLOW_COPY_AND_MOVE(Socket);

    void bind(InetAddress *addr);
    void listen();
    int accept(InetAddress *addr);

    void connect(InetAddress *addr);
    void connect(const char *ip, uint16_t port);

    void setNonBlocking();
    bool isNonBlocking();
    int getFd();
};