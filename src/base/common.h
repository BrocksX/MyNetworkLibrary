#pragma once
#include <cassert>
#include <stdio.h>

class Server;
class EventLoop;
class Epoll;
class Acceptor;
class Connection;
class Channel;
class Socket;
class Buffer;
class ThreadPool;

#define DISALLOW_COPY(cname)     \
  cname(const cname &) = delete; \
  cname &operator=(const cname &) = delete;

#define DISALLOW_MOVE(cname) \
  cname(cname &&) = delete;  \
  cname &operator=(cname &&) = delete;

#define DISALLOW_COPY_AND_MOVE(cname) \
  DISALLOW_COPY(cname);               \
  DISALLOW_MOVE(cname);