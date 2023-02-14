#include "RedisConnectPool.h"
#include "stdio.h"

void redis_test();
void pool_test();

int main()
{
    redis_test();
    return 0;
}

void pool_test()
{
    RedisConnectPool* pool = RedisConnectPool::getConnectionPool("127.0.0.1", 6379, 20);
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