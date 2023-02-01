#include "Socket.h"
#include <fcntl.h>
#include <sys/socket.h>
#include <unistd.h>
#include <cerrno>
#include <cstdio>
#include <cstring>
#include "util.h"

Socket::Socket()
{
    fd_ = socket(AF_INET, SOCK_STREAM, 0);
    errif(fd_ == -1, "socket create error");
}
Socket::Socket(int fd) : fd_(fd) 
{ 
    errif(fd_ == -1, "socket create error"); 
}

Socket::~Socket()
{
    if (fd_ != -1)
    {
        close(fd_);
    }
}

void Socket::bind(InetAddress *addr)
{
    struct sockaddr_in tmp_addr = addr->getAddr();
    errif(::bind(fd_, (sockaddr *)&tmp_addr, sizeof(tmp_addr)) == -1, "socket bind error");
}

void Socket::listen() { errif(::listen(fd_, SOMAXCONN) == -1, "socket listen error"); }

void Socket::setNonBlocking() { fcntl(fd_, F_SETFL, fcntl(fd_, F_GETFL) | O_NONBLOCK); }

bool Socket::isNonBlocking() { return (fcntl(fd_, F_GETFL) & O_NONBLOCK) != 0; }

int Socket::accept(InetAddress *addr)
{
    int clnt_sockfd = -1;
    sockaddr_in tmp_addr{};
    socklen_t addr_len = sizeof(tmp_addr);
    if (isNonBlocking())
    {
        while (true)
        {
            clnt_sockfd = ::accept(fd_, (sockaddr *)&tmp_addr, &addr_len);
            if (clnt_sockfd == -1 && ((errno == EAGAIN) || (errno == EWOULDBLOCK)))
            {
                continue;
            }
            if (clnt_sockfd == -1)
            {
                errif(true, "socket accept error");
            }
            else
            {
                break;
            }
        }
    }
    else
    {
        clnt_sockfd = ::accept(fd_, (sockaddr *)&tmp_addr, &addr_len);
        errif(clnt_sockfd == -1, "socket accept error");
    }
    addr->setAddr(tmp_addr);
    return clnt_sockfd;
}

void Socket::connect(InetAddress *addr)
{
    // for client socket
    struct sockaddr_in tmp_addr = addr->getAddr();
    if (fcntl(fd_, F_GETFL) & O_NONBLOCK)
    {
        while (true)
        {
            int ret = ::connect(fd_, (sockaddr *)&tmp_addr, sizeof(tmp_addr));
            if (ret == 0)
            {
                break;
            }
            if (ret == -1 && (errno == EINPROGRESS))
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
            if (ret == -1)
            {
                errif(true, "socket connect error");
            }
        }
    }
    else
    {
        errif(::connect(fd_, (sockaddr *)&tmp_addr, sizeof(tmp_addr)) == -1, "socket connect error");
    }
}

void Socket::connect(const char *ip, uint16_t port)
{
    InetAddress *addr = new InetAddress(ip, port);
    connect(addr);
    delete addr;
}

int Socket::getFd() { return fd_; }
