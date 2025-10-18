#pragma once

#include <mutex>
#include <condition_variable>
#include <thread>
#include <vector>
#include <functional>
#include <queue>
#include <stop_token>
#include <iostream>

class CThreadPool {
private:
    
    std::mutex _mutex; // To protect shared resources like the task queue.
    std::condition_variable _condVar; // To signal to waiting worker threads that new tasks are available.
    std::vector<std::jthread> _workers; // Worker threads that will run the task.
    std::queue<std::function<void()>> _taskQueue; // To hold tasks submitted to the pool.
    bool _stopping = false;
    
    void workerLoop(std::stop_token& stopToken);

public:

    // Constructor
    explicit CThreadPool(size_t numThreads);

    // Destructor
    ~CThreadPool() {
        shutdown();
    };

    CThreadPool(const CThreadPool&) = delete;             // No copy constructor
    CThreadPool& operator=(const CThreadPool&) = delete;  // No copy assignment
    
    // Move Constructor
    CThreadPool(CThreadPool&& other) noexcept {    
        // Mutexes and conditionVars are not movable
        this->_workers = std::move(other._workers);
        this->_taskQueue = std::move(other._taskQueue);
        this->_stopping = std::exchange(other._stopping, true); // Prevent the source object's destructor from joining
    }

    // Move assignment
    CThreadPool& operator=(const CThreadPool&& other) noexcept {
        // Mutexes and conditionVars are not movable
        this->_workers = std::move(other._workers);
        this->_taskQueue = std::move(other._taskQueue);
        this->_stopping = std::exchange(other._stopping, true); // Prevent the source object's destructor from joining
        return *this;
    }

    // Enqueue operation to submit task to CThreadPool
    template<class F>
    void enqueue(F&& f);

    // Close all threads
    void shutdown();
};

static_assert(!std::is_copy_constructible_v<CThreadPool>);
static_assert(!std::is_copy_assignable_v<CThreadPool>);
static_assert(std::is_nothrow_move_constructible_v<CThreadPool>);
static_assert(std::is_nothrow_move_assignable_v<CThreadPool>);