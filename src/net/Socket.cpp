#include "Socket.h"
#include "util.h"
#include <fcntl.h>
#include <sys/socket.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>

Socket::Socket() : fd(-1)
{
    fd = socket(AF_INET, SOCK_STREAM, 0);
    errif(fd == -1, "socket creator error");
}
Socket::Socket(int _fd) : fd(_fd)
{
    errif(fd == -1, "socket creat error");
}
Socket::~Socket()
{
    if (fd != -1)
    {
        close(fd);
        fd = -1;
    }
}
void Socket::bind(InetAddress *_addr)
{
    sockaddr_in addr = _addr->getAddr();
    socklen_t addr_len = _addr->getAddr_len();
    errif(::bind(fd, (sockaddr *)&addr, addr_len) == -1, "socket bind error");
}
void Socket::listen()
{
    errif(::listen(fd, SOMAXCONN) == -1, "socket listen error");
}
void Socket::setnonblocking()
{
    fcntl(fd, F_SETFL, fcntl(fd, F_GETFL) | O_NONBLOCK);
}
int Socket::accept(InetAddress *_addr)
{
    int clnt_sockfd = -1;
    sockaddr_in addr;
    socklen_t addr_len = sizeof(addr);
    bzero(&addr, sizeof(addr));
    if (fcntl(fd, F_GETFL) & O_NONBLOCK)
    {
        while (true)
        {
            clnt_sockfd = ::accept(fd, (sockaddr *)&addr, &addr_len);
            if (clnt_sockfd == -1 && ((errno == EAGAIN) || (errno == EWOULDBLOCK)))
            {
                // printf("no connection yet\n");
                continue;
            }
            else if (clnt_sockfd == -1)
            {
                errif(true, "socket accept error");
            }
            else
                break;
        }
    }
    else
    {
        clnt_sockfd = ::accept(fd, (sockaddr *)&addr, &addr_len);
        errif(clnt_sockfd == -1, "socket accept error");
    }
    _addr->setInetAddr(addr, addr_len);
    return clnt_sockfd;
}
int Socket::getFd()
{
    return fd;
}
void Socket::connect(InetAddress *_addr)
{
    sockaddr_in addr = _addr->getAddr();
    if (fcntl(fd, F_GETFL) & O_NONBLOCK)
    {
        while (true)
        {
            int ret = ::connect(fd, (sockaddr *)&addr, sizeof(addr));
            if (ret == 0)
            {
                break;
            }
            else if (ret == -1 && (errno == EINPROGRESS))
            {
                continue;
                /* 连接非阻塞式sockfd建议的做法：
                    The socket is nonblocking and the connection cannot be
                  completed immediately.  (UNIX domain sockets failed with
                  EAGAIN instead.)  It is possible to select(2) or poll(2)
                  for completion by selecting the socket for writing.  After
                  select(2) indicates writability, use getsockopt(2) to read
                  the SO_ERROR option at level SOL_SOCKET to determine
                  whether connect() completed successfully (SO_ERROR is
                  zero) or unsuccessfully (SO_ERROR is one of the usual
                  error codes listed here, explaining the reason for the
                  failure).
                  这里为了简单、不断连接直到连接完成，相当于阻塞式
                */
            }
            else if (ret == -1)
            {
                errif(true, "socket connect error");
            }
        }
    }
    else
    {
        errif(::connect(fd, (sockaddr *)&addr, sizeof(addr)) == -1, "socket connect error");
    }
}
bool Socket::isNonBlocking() 
{ 
    return (fcntl(fd, F_GETFL) & O_NONBLOCK) != 0; 
}
void Socket::connect(const char *ip, uint16_t port) {
  InetAddress *addr = new InetAddress(ip, port);
  connect(addr);
  delete addr;
}

InetAddress::InetAddress() : addr_len(sizeof(addr))
{
    bzero(&addr, sizeof(addr));
}
InetAddress::InetAddress(const char *ip, uint16_t port) : addr_len(sizeof(addr))
{
    bzero(&addr, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = inet_addr(ip);
    addr.sin_port = htons(port);
    addr_len = sizeof(addr);
}
InetAddress::~InetAddress() {}
void InetAddress::setInetAddr(sockaddr_in _addr, socklen_t _addr_len)
{
    addr = _addr;
    addr_len = _addr_len;
}
sockaddr_in InetAddress::getAddr()
{
    return addr;
}
socklen_t InetAddress::getAddr_len()
{
    return addr_len;
}