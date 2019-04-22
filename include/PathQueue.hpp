#pragma once

#include <mutex>
#include <queue>
#include <experimental/filesystem>

class PathQueue {
public:
    void push(std::experimental::filesystem::path new_path);
    std::experimental::filesystem::path pop();
    bool empty();

private:
    std::queue<std::experimental::filesystem::path> path_queue;
    std::mutex queue_mutex;
};
