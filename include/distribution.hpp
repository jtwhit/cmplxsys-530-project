#pragma once

#include <set>
#include <utility>

int uniform_int(int min, int max);

double uniform_real(double min, double max);

std::pair<int, std::set<int>> generate_info(int info_min, int info_max, int length, double std_dev);
