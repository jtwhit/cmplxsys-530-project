#include "SimParams.hpp"
#include <set>
#include <fstream>

using namespace std;

const string DEFAULT_PARAM_PATH = "params/default.param";

const set<Param> INT_PARAMS = {
    num_users,
    num_pages,
    max_info_int,
    page_length,
    page_length_min,
    page_length_max,
    user_length,
    user_length_min,
    user_length_max
};

const map<string, Param> PARAM_KEYS = {
    { "NUM_USERS", num_users },
    { "NUM_PAGES", num_pages },
    { "MAX_INFO_INT", max_info_int },
    { "PAGE_LENGTH", page_length },
    { "PAGE_LENGTH_MIN", page_length_min },
    { "PAGE_LENGTH_MAX", page_length_max },
    { "PAGE_STD_DEV", page_std_dev },
    { "PAGE_STD_DEV_MIN", page_std_dev_min },
    { "PAGE_STD_DEV_MAX", page_std_dev_max },
    { "PAGE_CLICK_WEIGHT", page_click_weight },
    { "INFO_FOUND_WEIGHT", info_found_weight },
    { "TOPIC_SIMILARITY_WEIGHT", topic_similarity_weight },
    { "USER_LENGTH", user_length },
    { "USER_LENGTH_MIN", user_length_min },
    { "USER_LENGTH_MAX", user_length_max },
    { "USER_STD_DEV", user_std_dev },
    { "USER_STD_DEV_MIN", user_std_dev_min },
    { "USER_STD_DEV_MAX", user_std_dev_max },
    { "USER_SAT_PCT", user_sat_pct },
    { "USER_SAT_PCT_MIN", user_sat_pct_min },
    { "USER_SAT_PCT_MAX", user_sat_pct_max }
};

 Param get_param(const string &key) {
     return PARAM_KEYS.at(key);
 }

bool is_integer_param(Param param) {
    return INT_PARAMS.count(param) > 0;
}

SimParams::SimParams() {
    read_file(DEFAULT_PARAM_PATH);
}

SimParams::SimParams(const string &param_path) :
    SimParams() {
    read_file(param_path);
}

void SimParams::set(Param param, int value) {
    switch (param) {
        case page_length:
            int_params[page_length_min] = value;
            int_params[page_length_max] = value;
            break;
        case user_length:
            int_params[user_length_min] = value;
            int_params[user_length_max] = value;
            break;
        default:
            int_params[param] = value;
            break;
    }
}

void SimParams::set(Param param, double value) {
    switch (param) {
        case page_std_dev:
            real_params[page_std_dev_min] = value;
            real_params[page_std_dev_max] = value;
            break;
        case user_std_dev:
            real_params[user_std_dev_min] = value;
            real_params[user_std_dev_max] = value;
            break;
        case user_sat_pct:
            real_params[user_sat_pct_min] = value;
            real_params[user_sat_pct_max] = value;
            break;
        default:
            real_params[param] = value;
            break;
    }
}

double SimParams::get_int(Param param) const {
    return int_params.at(param);
}

double SimParams::get_real(Param param) const {
    return real_params.at(param);
}

void SimParams::read_file(const string &path) {
    ifstream input(path);

    string param_key;
    while (input >> param_key) {
        Param param_type = get_param(param_key);
        if (is_integer_param(param_type)) {
            int value;
            input >> value;
            set(param_type, value);
        } else {
            double value;
            input >> value;
            set(param_type, value);
        }
    }
}
