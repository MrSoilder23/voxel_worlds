#include "./utility/thread_pool.hpp"

ThreadPool::ThreadPool(size_t numThreads) : stop(false) {
    for (size_t i = 0; i < numThreads; i++) {
        workers.emplace_back([this] {
            while(true) {
                std::unique_lock<std::mutex> lock(queueMutex);
                condition.wait(lock, [this] {return stop || !tasks.empty(); });
                if(stop && tasks.empty()) {
                    return;
                }
                auto task = std::move(tasks.front());
                tasks.pop();
                lock.unlock();
                task();
            }
        });
    }
}

ThreadPool::~ThreadPool() {
    std::unique_lock<std::mutex> lock(queueMutex);
    stop = true;
    lock.unlock();
    condition.notify_all();
    for(std::thread& worker : workers) {
        worker.join();
    }
}

ThreadPool& ThreadPool::GetInstance() {
    static ThreadPool sInstance(VoxelWorlds::THREAD_AMOUNT);
    return sInstance;
}
