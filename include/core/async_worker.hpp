#pragma once
#include "core/thread_pool.hpp"

namespace http {
namespace core {

class AsyncWorker {
public:
    AsyncWorker(ThreadPool& pool) : pool_(pool) {}
    
    template<class F, class... Args>
    auto run(F&& f, Args&&... args) {
        return pool_.enqueue(std::forward<F>(f), std::forward<Args>(args)...);
    }

private:
    ThreadPool& pool_;
};

}
}
