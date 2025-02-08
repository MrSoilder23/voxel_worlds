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

        template<class F>
        void enqueue(F&& task) {
            std::unique_lock<std::mutex> lock(queueMutex);
            tasks.emplace(std::forward<F>(task));
            lock.unlock();
            condition.notify_one();
        }

        template<class C, class F, class... Args>
        void enqueue(C* obj, F&& f, Args&&... args) {
            auto task = std::bind(std::forward<F>(f), obj, std::forward<Args>(args)...);
            
            std::unique_lock<std::mutex> lock(queueMutex);
            tasks.emplace([task = std::move(task)]() { task(); });
        
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
