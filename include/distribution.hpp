#pragma once

#include <set>
#include <utility>

std::pair<int, std::set<int>> generate_info(int info_min, int info_max, int length, double std_dev);
