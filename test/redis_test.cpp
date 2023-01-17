#include <iostream>
#include <algorithm>
#include <vector>
#include "Timestamp.h"
#include "RedisConnectPool.h"

using namespace std;

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
}
/*
int main()
{
    Timestamp *t = new Timestamp();
    int64_t aa = t->now().getSecondsSinceEpoch();
    cout<< aa <<endl;
    return 0;
}
*/
