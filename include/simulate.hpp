#pragma once

#include "SearchEngine.hpp"
#include <vector>
#include <mutex>
#include <iostream>

struct SimParams {
    int num_users;
    int num_pages;
    int max_info_int;
    int page_length;
    double page_std_dev;
    Weights weights;
    int user_length;
    double user_std_dev;
    double user_sat_pct;
};

class SimProgress {
public:
    SimProgress(const std::string &name_);

    void set_target(int target);
    void increment();
    bool working();

    friend std::ostream& operator<<(std::ostream &output, SimProgress &progress);
private:
    std::string name;
    int total_ops = 0, done_ops = 0;
    std::mutex progress_mutex;
};

struct SimResult {
    int list_depth, pages_read;
};

std::vector<SimResult> simulate(SimParams params, SimProgress &progress);
