#pragma once

#include <string>
#include <vector>

class Display;

// Weights given to different types of data. Absolute magnitude of weights does
// not matter, only their magnitudes relative to each other.
struct Weights {
    double page_click, info_found, topic_similarity;
};

struct SimParams {
    std::string name;
    int num_users;
    int num_pages;
    int max_info_int;
    int page_length_min, page_length_max;
    double page_std_dev_min, page_std_dev_max;
    Weights weights;
    int user_length_min, user_length_max;
    double user_std_dev_min, user_std_dev_max;
    double user_sat_pct_min, user_sat_pct_max;
};


struct SimResult {
    int list_depth, pages_read;
};

std::vector<SimResult> simulate(SimParams params, Display &display);
