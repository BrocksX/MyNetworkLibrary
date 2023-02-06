#include <iostream>
#include <algorithm>
#include <vector>
#include <functional>
#include "EventLoop.h"
#include "Timestamp.h"
#include "RedisConnectPool.h"
#include "Timer.h"

using namespace std;
/*
int main()
{
    RedisConnectPool *redisConns = new RedisConnectPool("127.0.0.1", 6379, 20);
    redisConns->connect();

    string ret = "";

    Redis *red = redisConns->getConnect();

    cout << red->hexists("k", "1") << endl;
    //cout << ret << endl;
    redisConns->releaseConnect(red);

    delete redisConns;
}*/

int main()
{
    EventLoop *loop = new EventLoop();
    Timestamp time;
    time = addTime(time.now(), 5);
    Timer* t = loop->runAt(time, [](){cout<<"cnm"<<endl;});
    loop->cancel(t);
    loop->loop();
    return 0;
}
