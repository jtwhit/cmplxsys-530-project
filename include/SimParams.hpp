#pragma once

#include <string>
#include <map>

enum Param {
    num_users,
    num_pages,
    max_info_int,
    page_length,
    page_length_min,
    page_length_max,
    page_std_dev,
    page_std_dev_min,
    page_std_dev_max,
    page_click_weight,
    info_found_weight,
    topic_similarity_weight,
    user_length,
    user_length_min,
    user_length_max,
    user_std_dev,
    user_std_dev_min,
    user_std_dev_max,
    user_sat_pct,
    user_sat_pct_min,
    user_sat_pct_max
};

Param get_param(const std::string &key);
bool is_integer_param(Param param);

class SimParams {
public:
    SimParams();
    SimParams(const std::string &param_path);

    void set(Param param, int value);
    void set(Param param, double value);

    double get_int(Param param) const;
    double get_real(Param param) const;

private:
    void read_file(const std::string &path);

    std::map<Param, int> int_params;
    std::map<Param, double> real_params;
};
