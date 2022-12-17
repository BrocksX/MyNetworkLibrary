#pragma once
#include "common.h"
#include <functional>

class EventLoop;
class Socket;
class Channel;
class Buffer;
class Connection
{
public:
    enum State
    {
        Invalid = 1,
        Handshaking,
        Connected,
        Closed,
        Failed,
    };
    Connection(EventLoop *loop, Socket *sock);
    ~Connection();
    DISALLOW_COPY_AND_MOVE(Connection);

    void read();
    void write();

    void setDeleteConnectionCallback(std::function<void(Socket *)> const &callback);
    void setOnConnectCallback(std::function<void(Connection *)> const &callback);
    State getState();
    void close();
    void setSendBuffer(const char *str);
    Buffer *getReadBuffer();
    const char *readBuffer();
    Buffer *getSendBuffer();
    const char *sendBuffer();
    void getlineSendBuffer();
    Socket *getSocket();

    void onConnect(std::function<void()> fn);

private:
    EventLoop *loop_;
    Socket *sock_;
    Channel *channel_{nullptr};
    State state_{State::Invalid};
    Buffer *readBuffer_{nullptr};
    Buffer *sendBuffer_{nullptr};
    std::function<void(Socket *)> deleteConnectioinCallback_;

    std::function<void(Connection *)> on_connect_callback_;

    void readNonBlocking();
    void writeNonBlocking();
    void readBlocking();
    void writeBlocking();
};