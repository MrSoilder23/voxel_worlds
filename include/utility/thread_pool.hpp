#pragma once
// C++ standard libraries
#include <thread>
#include <vector>
#include <queue>
#include <mutex>
#include <future>
#include <functional>   
#include <condition_variable>
// Own libraries
#include "constant.hpp"

class ThreadPool {
    public:
        ThreadPool(size_t numThreads);
        ~ThreadPool();

        void Shutdown();

        template<class F>
        void enqueue(F&& task) {
            std::unique_lock<std::mutex> lock(queueMutex);
            tasks.emplace(std::forward<F>(task));
            lock.unlock();
            condition.notify_one();
        }

        static ThreadPool& GetInstance();

    private:
        std::vector<std::thread> workers;
        std::queue<std::function<void()>> tasks;

        std::mutex queueMutex;
        std::condition_variable condition;
        bool stop;
};
