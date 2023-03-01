# Channel 分析



### Channel与文件描述符

一个Channel管理一个文件描述符，并与一个EventLoop绑定

```c++
class Channel
{
    ...
private:
    EventLoop *loop_;
    const int fd_;
	...
};
```

该文件描述符的关注事件表示为

```c++
const int Channel::kNoneEvent = 0;
const int Channel::kReadEvent = EPOLLIN | EPOLLPRI;
const int Channel::kWriteEvent = EPOLLOUT;
```

在Channel内部定义了变量来标记文件描述符所关注的事件以及实际发生的事件

```c++
uint32_t listenEvents_;
uint32_t readyEvents_;
```

### 启动监听

```c++
void Channel::enableReading()
{
    listenEvents_ |= kReadEvent;
    loop_->updateChannel(this);
}
```

在EventLoop中更改这个Channel，EventLoop会调用Epoll的相关函数，执行`epoll_ctl`操作，更改监听事件



### 设置监听回调函数

```c
    void setReadCallback(std::function<void()> const &callback);
    void setWriteCallback(std::function<void()> const &callback);
```



### 响应事件

Channel可以响应事件，并调用对用的回调函数

```c++
void Channel::handleEvent()
{
    if (readyEvents_ & kReadEvent)
    {
        readCallback_();
    }
    if (readyEvents_ & kWriteEvent)
    {
        writeCallback_();
    }
}
```



to do list

支持更多响应事件

2023/2/17
