#pragma once

#include <random>
#include <vector>

extern std::default_random_engine random_gen;

int uniform_int(int min, int max);
std::vector<int> normal_ints(int n, int mean, double standard_dev);
