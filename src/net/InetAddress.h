#include <arpa/inet.h>
#include <cstring>
#include "nocopyable.h"

class InetAddress
{
public:
    DISALLOW_COPY_AND_MOVE(InetAddress);

    InetAddress() = default;
    InetAddress(const char *ip, uint16_t port);
    ~InetAddress() = default;

    void setAddr(sockaddr_in addr);
    sockaddr_in getAddr();
    const char *getIp();
    uint16_t getPort();

private:
    sockaddr_in addr_{};
};