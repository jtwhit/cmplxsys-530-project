#pragma once

#include "SearchEngine.hpp"
#include <vector>

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

const SimParams DEFAULT_PARAMS =
    { 3000,
      100000,
      10000,
      100,
      80,
      { 1.0/3.0, 1.0/3.0, 1.0/3.0 },
      50,
      10,
      0.95 };

struct SimResult {
    int list_depth, pages_read;
};

std::vector<SimResult> simulate(SimParams params, bool print = false);
