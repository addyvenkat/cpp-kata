#include "threadPool.hpp"

// constructor
CThreadPool::CThreadPool(size_t numThreads) {
    if(numThreads == 0) {
        throw std::invalid_argument("ThreadPool must have at least one thread.\n");
    }
    for(int i = 0; i < numThreads; i++) {
        std::stop_token stopToken;
        _workers.emplace_back(&CThreadPool::workerLoop, this, stopToken);
    }
}

// Worker loop for each jthread
void CThreadPool::workerLoop(std::stop_token& stopToken) {
    while(true) {
        std::function<void()> task;
        {
            std::unique_lock lk(_mutex)
            _condVar.wait(lk, [this]{ 
                return (!_taskQueue.empty() || st.stop_requested() || _stopping);
            });

            if(_taskQueue.empty() && (st.stop_requested() || _stopping)) {
                // Exit condition for worker thread
                return;
            }

            auto task = std::move(_taskQueue.front());
            _taskQueue.pop();
        }

        try
        {
            // Perform task
            task();
        }
        catch(const std::exception& e)
        {
            std::cerr << e.what() << '\n';
        }
    }
}

// Enqueue operation to submit tasks to CThreadPool
template<class F>
void CThreadPool::enqueue(F&& f) {
    std::lock_guard lk(_mutex);
    if(_stopping) {
        throw std::runtime_error("enqueueing on stopped ThreadPool\n");
    }
    _taskQueue.emplace(std::forward<F>(f));
}


// Destructor
CThreadPool::shutdown() {
    {
        std::lock_guard lk(_mutex);
        _stopping = true;
    }
    
    for (auto& jt : _workers) {
        jt.request_stop();
    }
    _condVar.notify_all();
    
    for (auto& jt : _workers) {
        if (jt.joinable()) {
            jt.join();
        }
    }
    _workers.clear();
}
