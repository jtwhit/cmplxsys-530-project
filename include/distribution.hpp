#pragma once

#include <set>
#include <utility>

// Generate normal distribution of given length and standard deviation
// consisting of unique integers centered randomly between info_min and info_max.
std::pair<int, std::set<int>> generate_info(int info_min, int info_max, int length, double std_dev);
