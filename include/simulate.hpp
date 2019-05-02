#pragma once

#include "SimParams.hpp"
#include <vector>

struct SimResult {
    int list_depth, pages_read;
};

std::vector<SimResult> simulate(SimParams params);
