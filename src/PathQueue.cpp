#include "PathQueue.hpp"
#include <iostream>

using namespace std;
using namespace std::experimental::filesystem;

void PathQueue::push(path new_path) {
    lock_guard<mutex> queue_lock(queue_mutex);
    path_queue.push(new_path);
}

path PathQueue::pop() {
    lock_guard<mutex> queue_lock(queue_mutex);
    path popped_path = path_queue.front();
    path_queue.pop();
    return popped_path;
}

bool PathQueue::empty() {
    lock_guard<mutex> queue_lock(queue_mutex);
    return path_queue.empty();
}
