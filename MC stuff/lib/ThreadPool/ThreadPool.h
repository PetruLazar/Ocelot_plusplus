// Original version by André Henriques
// This is a modified version

#pragma once

#include <algorithm>
#include <functional>
#include <future>
#include <queue>
#include <utility>

class ThreadPool {
private:
    std::vector<std::thread> threads_;
    std::queue<std::packaged_task<void()>> tasks_;
    std::mutex mutex_;
    std::condition_variable condition_;
    bool stopped_;

public:
    ThreadPool(std::size_t numberOfThreads) : stopped_(false) {
        threads_.reserve(numberOfThreads);
        for (std::size_t i = 0; i < numberOfThreads; ++i) {
            threads_.emplace_back(
                [this] {
                    while (true) {
                        std::packaged_task<void()> task;
                        {
                            std::unique_lock<std::mutex> uniqueLock(mutex_);
                            condition_.wait(
                                uniqueLock,
                                [this] {
                                    return tasks_.empty() == false || stopped_ == true;
                                }
                            );
                            if (tasks_.empty() == false) {
                                task = std::move(tasks_.front());
                                // attention! tasks_.front() moved
                                tasks_.pop();
                            }
                            else // stopped_ == true (necessarily)
                                return;
                        }
                        task();
                    }
                }
            );
        }
    }

    // joins all threads
    ~ThreadPool() {
        {
            std::lock_guard<std::mutex> lockGuard(mutex_);
            stopped_ = true;
        }
        condition_.notify_all();
        for (std::thread& worker : threads_) {
            worker.join();
        }
    }

    template<class F, class... A>
    decltype(auto) enqueue(F&& callable, A &&...arguments) {
        using ReturnType = std::invoke_result_t<F, A...>;
        auto task = std::make_shared< std::packaged_task<ReturnType()> >(
            std::bind(std::forward<F>(callable), std::forward<A>(arguments)...));

        std::future<ReturnType> taskFuture = task->get_future();
        {
            std::unique_lock<std::mutex> lock(mutex_);
            tasks_.emplace([task]() { (*task)(); });
            // attention! task moved
        }
        condition_.notify_one();
        return taskFuture;
    }
};
