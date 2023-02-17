#include <iostream>
#include <algorithm>
#include <vector>
#include <functional>
#include "EventLoop.h"
#include "Timestamp.h"
#include "RedisConnectPool.h"
#include "Timer.h"
EventLoop *g_loop;
using namespace std;

// void cancel(std::shared_ptr<Timer> timer)
// {
//     g_loop->cancel(timer);
// }
// int main()
// {
//     int g = 0;
//     int N = 3500000;
//     EventLoop loop;
//     g_loop = &loop;
//     for(int i = 0; i < N; ++i)
//     {
//         std::shared_ptr<Timer> t = loop.runEvery(1.0, [&g](){g++;});
//         loop.runAfter(4.5, std::bind(cancel, t));
//     }
    
//     loop.runAfter(5.5, [&g](){printf("%d\n", g);});
//     loop.runAfter(10.5, [&g](){printf("%d\n", g);});
//     loop.runAfter(15.5, [&g](){printf("%d\n", g);});

//     loop.loop();
//     return 0;
// }

void print(const char* msg)
{
  printf("msg %s %s\n", Timestamp::now().toString().c_str(), msg);
}
void cancel(std::shared_ptr<Timer> timer)
{
  g_loop->cancel(timer);
  printf("cancelled at %s\n", Timestamp::now().toString().c_str());
}
int main()
{
    EventLoop loop;
    g_loop = &loop;

    print("main");
    std::shared_ptr<Timer> t1 = loop.runAfter(1, std::bind(print, "once1"));
    loop.runAfter(1.5, std::bind(print, "once1.5"));
    loop.runAfter(2.5, std::bind(print, "once2.5"));
    loop.runAfter(3.5, std::bind(print, "once3.5"));
    std::shared_ptr<Timer> t45 = loop.runAfter(4.5, std::bind(print, "once4.5"));
    loop.runAfter(4.2, std::bind(cancel, t45));
    loop.runAfter(4.8, std::bind(cancel, t45));
    loop.runEvery(2, std::bind(print, "every2"));
    std::shared_ptr<Timer> t3 = loop.runEvery(3, std::bind(print, "every3"));
    loop.runAfter(9.001, std::bind(cancel, t3));

    loop.loop();
    print("main loop exits");
}
