#pragma once
#include "noncopyable.h"
#include <functional>
#include <string>
#include <memory>
#include "EventLoop.h"
#include "FileHandler.h"

class EventLoop;
class Socket;
class Channel;
class Buffer;
/**
 * Acceptor类的负责新建连接，Connection类负责处理事件的逻辑。
 * 通过绑定回调函数实现业务逻辑
*/
class Connection : public std::enable_shared_from_this<Connection>
{
public:
    DISALLOW_COPY_AND_MOVE(Connection);
    enum State
    {
        Invalid = 1,
        Handshaking,
        Connected,
        Closed,
        Failed,
    };
    Connection(EventLoop *loop, Socket *socket);
    ~Connection();

    //读写操作
    void read();
    void send(const std::string &msg);
    void send(const Buffer &buffer);
    void send(const char *msg);
    void shutdown();
    void close();

    void sendFile(FileHandler* file);

    void connectEstablished();
    void connectDestroyed();

    void setDeleteConnectionCallback(std::function<void(std::shared_ptr<Connection>)> const &callback);
    void setMessageCallback(std::function<void(std::shared_ptr<Connection>)> const &callback);

    State getState();
    void setSendBuffer(const char *str);
    Buffer *getReadBuffer();
    const char *readBuffer();
    Buffer *getSendBuffer();
    const char *sendBuffer();
    void getlineSendBuffer();
    Socket *getSocket();
    EventLoop *getLoop() const
    {return loop_;}
    int getFd() const
    {return fd_;}


private:
    EventLoop *loop_;
    std::unique_ptr<Socket> socket_;
    std::unique_ptr<Channel> channel_;
    State state_;
    int fd_;
    size_t fileWrote_;
    FileHandler *file_;
    std::unique_ptr<Buffer> readBuffer_;
    std::unique_ptr<Buffer> sendBuffer_;
    std::function<void(std::shared_ptr<Connection>)> deleteConnectioinCallback_;
    std::function<void(std::shared_ptr<Connection>)> messageCallback_;
    std::function<void(std::shared_ptr<Connection>)> writeCompleteCallback_;

    void write();
    void readNonBlocking();
    void writeNonBlocking();
    void readBlocking();
    void writeBlocking();

    void sendInLoop(const char *str);
    void sendFileInLoop(FileHandler* file);
};