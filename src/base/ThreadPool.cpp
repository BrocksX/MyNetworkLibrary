#include "ThreadPool.h"

ThreadPool::ThreadPool(int size) : size_(size), stop_(false)
{
    for (int i = 0; i < size_; ++i)
    {
        threads_.emplace_back(std::thread([this]()
        {
            while (true)
            {
                std::function<void()> task;
                {
                    std::unique_lock<std::mutex> lock(queue_mtx_);
                    cv_.wait(lock, [this]()
                            { return stop_ || !tasks_.empty(); });
                    if (stop_ && tasks_.empty())
                        return;
                    task = tasks_.front();
                    tasks_.pop();
                }
                task();
            }
        }));
    }
}
ThreadPool::~ThreadPool()
{
    {
        std::unique_lock<std::mutex> lock(queue_mtx_);
        stop_ = true;
    }
    cv_.notify_all();
    for(std::thread &th : threads_)
    {
        if(th.joinable())   
            th.join();
    }
}
