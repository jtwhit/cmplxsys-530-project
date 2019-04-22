#pragma once

#include "SearchEngine.hpp"
#include "Display.hpp"
#include <string>
#include <vector>

struct SimParams {
    std::string name;
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

struct SimResult {
    int list_depth, pages_read;
};

std::vector<SimResult> simulate(SimParams params, Display &display);
