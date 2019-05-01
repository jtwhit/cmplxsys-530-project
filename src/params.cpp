#include "params.hpp"
#include <fstream>
#include <experimental/filesystem>

using namespace std;
using namespace std::experimental::filesystem;

SimParams read_params(const string &param_path) {
    SimParams params;
    params.name = param_path;

    ifstream input(param_path);
    input >> params.num_users;
    input >> params.num_pages;
    input >> params.max_info_int;
    input >> params.page_length_min >> params.page_length_max;
    input >> params.page_std_dev_min >> params.page_std_dev_max;
    input >> params.weights.page_click >> params.weights.info_found >> params.weights.topic_similarity;
    input >> params.user_length_min >> params.user_length_max;
    input >> params.user_std_dev_min >> params.user_std_dev_max;
    input >> params.user_sat_pct_min >> params.user_sat_pct_max;

    return params;
}
