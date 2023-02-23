#include "EventLoop.h"
#include "ThreadPool.h"

void print(EventLoop *p = NULL)
{
    printf("print: pid = %d, tid = %d, loop = %p\n", getpid(), CurrentThread::tid(), p);
}
void quit(EventLoop *p)
{
    print(p);
    p->quit();
}
void create_eventloop()
{
    print();
    EventLoop *loop = new EventLoop();
    loop->runInLoop(std::bind(print, loop));
    loop->loop();
}
int main()
{
    std::thread t1(create_eventloop);
    std::thread t2(create_eventloop);

    t1.join();
    t2.join();
}
