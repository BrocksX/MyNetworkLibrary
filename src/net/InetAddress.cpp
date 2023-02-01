#include "InetAddress.h"

InetAddress::InetAddress(const char *ip, uint16_t port)
{
    memset(&addr_, 0, sizeof(addr_));
    addr_.sin_family = AF_INET;
    addr_.sin_addr.s_addr = inet_addr(ip);
    addr_.sin_port = htons(port);
}


void InetAddress::setAddr(sockaddr_in addr) { addr_ = addr; }

sockaddr_in InetAddress::getAddr() { return addr_; }

const char *InetAddress::getIp() { return inet_ntoa(addr_.sin_addr); }

uint16_t InetAddress::getPort() { return ntohs(addr_.sin_port); }