#include "Socket.h"
#include <fcntl.h>
#include <sys/socket.h>
#include <unistd.h>
#include <cerrno>
#include <cstdio>
#include <cstring>
#include <stdexcept>

Socket::Socket()
{
    fd_ = socket(AF_INET, SOCK_STREAM, 0);
    if(fd_ == -1)   throw std::runtime_error("socket create error");
}
Socket::Socket(int fd) : fd_(fd) 
{ 
    if(fd_ == -1)   throw std::runtime_error("socket create error");
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
    if(::bind(fd_, (sockaddr *)&tmp_addr, sizeof(tmp_addr)) == -1)
        throw std::runtime_error("socket bind error");
}

void Socket::listen() 
{ 
    if(::listen(fd_, SOMAXCONN) == -1)
        throw std::runtime_error("socket listen error");
}

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
                throw std::runtime_error("socket accept error");
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
        if(clnt_sockfd == -1)
            throw std::runtime_error("socket accept error");
    }
    addr->setAddr(tmp_addr);
    return clnt_sockfd;
}

void Socket::connect(InetAddress *addr)
{
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
            }
            if (ret == -1)
            {
                throw std::runtime_error("socket connect error");
            }
        }
    }
    else
    {
        if(::connect(fd_, (sockaddr *)&tmp_addr, sizeof(tmp_addr)) == -1)
            throw std::runtime_error("socket connect error");
    }
}

void Socket::connect(const char *ip, const uint16_t &port)
{
    InetAddress *addr = new InetAddress(ip, port);
    connect(addr);
    delete addr;
}

int Socket::getFd() { return fd_; }
