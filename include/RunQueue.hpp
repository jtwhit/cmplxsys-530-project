#pragma once

#include "Run.hpp"
#include <mutex>
#include <condition_variable>
#include <queue>
#include <memory>
#include <atomic>

class RunQueue {
public:
    RunQueue();
    void push(std::shared_ptr<Run> run);
    void execute();

private:
    std::shared_ptr<Run> pop();
    void execute_worker();

    std::atomic_int running_threads;
    std::queue<std::shared_ptr<Run>> run_queue;
    std::mutex queue_mutex;
    std::condition_variable done_cv;
};
