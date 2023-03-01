# Poller模块

Poller类是来管理epoll fd的，一个Epoll对象管理一个epoll fd，初始化时创建fd。一个EventLoop关联一个Poller，Channel的update会调用EventLoop的update函数，而EventLoop又调用Poller相关的函数。EventLoop之所以能够“事件循环”也是其内部调用Poller的poll函数，所以有：

```c++
class Poller
{
	...
private:
    const int epfd_;
	...
};

Poller::Poller() : epfd_(epoll_create1(EPOLL_CLOEXEC))
{
    ...
}
```

### Poller与Channel

```c++
    void updateChannel(Channel*);
    void deleteChannel(Channel*);
```

EventList events_的作用非常明显，因为epoll函数需要一个struct epoll_event的数组地址，所以该结构是用于epoll_wait函数参数。ChannelMap channels_则是用于管理注册的Channel的，key是Channel对应的文件描述符fd，value就是该Channel的地址，使用map数据结构可以很方便的对Channel进行查找和操作。

### updateChannel和 deleteChannel

updateChannel函数定义如下，其核心是通过`epoll_ctl`操作epoll函数所生成的实例。

```c++
void Poller::updateChannel(Channel *channel)
{
    int fd = channel->getFd();
    epoll_event ev;
    bzero(&ev, sizeof(ev));
    ev.data.ptr = channel;
    ev.events = channel->getListenEvents();
    if (!channel->getInEpoll())
    {
        if (epoll_ctl(epfd_, EPOLL_CTL_ADD, fd, &ev) == -1)
            throw std::runtime_error("epoll add error");
        channel->setInEpoll();
    }
    else
    {
        if (epoll_ctl(epfd_, EPOLL_CTL_MOD, fd, &ev) == -1)
            throw std::runtime_error("epoll modify error");
    }
}
```

deleteChannel函数定义如下，该函数将取消对Channel对应的文件描述的事件监听，然后将该Channel从channels_中删除。

```c++
void Poller::deleteChannel(Channel *channel)
{
    int fd = channel->getFd();
    if (epoll_ctl(epfd_, EPOLL_CTL_DEL, fd, NULL) == -1)
        throw std::runtime_error("epoll delete error");
    channel->setInEpoll(false);
    close(fd);
}
```



### poll 

poll函数是在EventLoop中调用的，EventLoop通过该函数获取到当前的活跃的Channel集合。poll函数会获取所有活跃的Channel，并返回一个vector集合，保存Channel的指针。

```c++
std::vector<Channel *> Poller::poll(int timeout)
{
    std::vector<Channel *> activeChannels;
    int nfds = epoll_wait(epfd_, events_, kMaxevents, timeout);
    if (nfds == -1)     throw std::runtime_error("epoll wait error");
    for (int i = 0; i < nfds; ++i)
    {
        Channel *ch = (Channel *)events_[i].data.ptr;
        ch->setReadyEvents(events_[i].events);
        activeChannels.emplace_back(ch);
    }
    return activeChannels;
}
```



