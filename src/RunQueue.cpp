#include "RunQueue.hpp"
#include <thread>

using namespace std;

RunQueue::RunQueue() {
    running_threads = 0;
}

void RunQueue::push(shared_ptr<Run> run) {
    unique_lock<mutex> queue_lock(queue_mutex);
    run_queue.push(run);
}

void RunQueue::execute() {
    unsigned int num_threads = thread::hardware_concurrency();
    for (unsigned int i = 0; i < num_threads; i++) {
        thread worker(&RunQueue::execute_worker, this);
        worker.detach();
    }

    unique_lock<mutex> queue_lock(queue_mutex);
    while (!run_queue.empty() || (running_threads > 0)) {
        done_cv.wait(queue_lock);
    }
}

shared_ptr<Run> RunQueue::pop() {
    unique_lock<mutex> queue_lock(queue_mutex);
    if (run_queue.empty()) {
        return nullptr;
    }

    shared_ptr<Run> popped = run_queue.front();
    run_queue.pop();
    return popped;
}

void RunQueue::execute_worker() {
    running_threads++;
    shared_ptr<Run> next_run;
    while (next_run = pop()) {
        next_run->execute();
    }
    running_threads--;
    done_cv.notify_all();
}
