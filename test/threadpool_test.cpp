#include "ThreadPool.h"
#include "stdio.h"

void task1()
{
    printf("TASK1 running\n");
}

void task2(int a)
{
    printf("TASK2 running print %d\n", a);
}

int task3(int a)
{
    printf("TASK3 running print %d\n", a);
    return a;
}

int main()
{
    ThreadPool pool;
    pool.add(task1);
    pool.add(task2, 5);
    std::future<int> ret = pool.add(task3, 10);
    printf("return is %d\n", ret.get());
    return 0;
}