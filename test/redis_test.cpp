#include "RedisConnectPool.h"
#include "stdio.h"
#include <chrono>
#include <thread>
#include <iostream>

using namespace std::chrono;
void redis_test();
void pool_test();

int main()
{
    pool_test();
    return 0;
}

void threadpool(RedisConnectPool* pool, int begin, int end)
{
    for (int i = begin; i < end; ++i)
    {
        Redis* conn = pool->getConnection();
        conn->set(std::to_string(i), "test");
        pool->releaseConnect(conn);
    }
}

void pool_test()
{
    RedisConnectPool* pool = RedisConnectPool::getConnectionPool("127.0.0.1", 6379, 20);
    steady_clock::time_point begin = steady_clock::now();
    std::thread t1(threadpool, pool, 0, 50000);
    std::thread t2(threadpool, pool, 50000, 100000);
    t1.join();
    t2.join();
    steady_clock::time_point end = steady_clock::now();
    auto length = end - begin;
    std::cout << "连接池用时: " << length.count() / 1000000 << " 毫秒" << std::endl;

}

void redis_test()
{
    Redis conn;
    conn.connect("127.0.0.1", 6379);

    printf("------SET TEST--------\n");
    conn.set("set test", "test");
    std::string value = conn.get("set test");
    printf("set test is %s\n", value.c_str());

    printf("------HASH TEST--------\n");
    std::unordered_map<std::string, std::string> str_str;
    str_str["f1"] = "v1";
    str_str["f2"] = "v2";
    str_str["f3"] = "v3";
    conn.hset("hash test", str_str);
    conn.hget("hash test", "f1", value);
    printf("f1 is %s\n", value.c_str());
    printf("is there exist f2? %s\n", conn.hexists("hash test", "f2")? "YES": "NO");
    std::vector<std::string> values;
    conn.hmget("hash test",{"f2", "f3"}, values);
    printf("f2 and f3 are ");
    for(auto const &v : values) printf("%s ", v.c_str());
    printf("\n");

    printf("------SORT SET TEST--------\n");
    std::unordered_map<int64_t, std::string> int_str;
    int_str[1] = "v1";
    int_str[2] = "v2";
    printf("add %ld values\n", conn.zadd("sort set test", int_str));
    printf("sort set list ");
    conn.zrange("sort set test", 0, 100, values, false);
    for(auto const &v : values) printf("%s ", v.c_str());
    printf("\n");
}