#pragma once

#include "params.hpp"
#include <vector>
#include <memory>

class Display;

struct SimResult {
    int list_depth, pages_read;
};

std::vector<SimResult> simulate(SimParams params, std::shared_ptr<Display> display = nullptr);
