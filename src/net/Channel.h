#pragma once
#include "common.h"
#include <functional>
#include <cstdint>
class Channel
{
public:
    Channel(EventLoop *loop, int fd);
    ~Channel();

    DISALLOW_COPY_AND_MOVE(Channel);

    void handleEvent();
    void enableRead();

    int getFd();
    uint32_t getListenEvents();
    uint32_t getReadyEvents();
    bool getInEpoll();
    void setInEpoll(bool in = true);
    void useET();

    void setReadyEvents(uint32_t ev);
    void setReadCallback(std::function<void()> const &callback);

private:
    EventLoop *loop_;
    int fd_;
    uint32_t listenEvents_;
    uint32_t readyEvents_;
    bool inEpoll_;
    std::function<void()> readCallback_;
    std::function<void()> writeCallback_;
};