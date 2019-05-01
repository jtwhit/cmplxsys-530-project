#include "User.hpp"
#include "distribution.hpp"
#include <algorithm>
#include <cmath>
#include <numeric>
#include <tuple>
#include <iostream>

using namespace std;

User::User(const SimParams &params_) :
    params(params_) {
    satisfied_pct = uniform_real(params.user_sat_pct_min, params.user_sat_pct_max);
    info_length = uniform_int(params.user_length_min, params.user_length_max);
    double std_dev = uniform_real(params.user_std_dev_min, params.user_std_dev_max);
    // Randomly choose topic and normally distribute info around it.
    tie(topic, search_info) = generate_info(params.max_info_int*0.2, params.max_info_int*0.8, info_length, std_dev);
}

ActionData User::read_page(double query, const WebPage &page) {
    // Find info in user's search info, but not in page's info.
    set<int> new_search_info;
    set<int> page_data = page.get_data();
    set_difference(search_info.begin(), search_info.end(),
                   page_data.begin(), page_data.end(),
                   inserter(new_search_info, new_search_info.begin()));

    int num_found = search_info.size() - new_search_info.size();
    search_info = new_search_info;
    read_pages.insert(page.get_id());

    //cout << "\t" << query << " " << page.get_id() << " " << page.get_topic() << " " << num_found << endl;
    // Record user action data.
    ActionData data;
    data.page_id = page.get_id();
    data.query = query;
    data.info_found = static_cast<double>(num_found) / page.get_length();
    double topic_distance = abs(query - page.get_topic());
    data.topic_similarity = (params.max_info_int - topic_distance) / params.max_info_int;

    return data;
}

int User::choose_page(double query, SearchEngine &search_engine) const {
    double max_score = 0;
    int max_index = -1;
    for (int i = 0; i < search_engine.get_num_pages(); i++) {        
        int page_rank = (i + 1);
        if ((1.0 / page_rank) < max_score && max_index >= 0) {
            break;
        }

        const WebPage& page = search_engine.get_page(i);
        if (read_pages.count(page.get_id()) != 0) {
            continue;
        }

        double distance = abs(query - page.get_topic());
        double score = 1 / ((distance + 1) * page_rank);
        if (score > max_score || max_index < 0) {
            max_score = score;
            max_index = i;
        }
    }

    return max_index;
}

double User::generate_query() const {
    double info_sum = accumulate(search_info.begin(), search_info.end(), 0.0);
    return info_sum / search_info.size();
}

bool User::is_satisfied() const {
    return (static_cast<double>(search_info.size()) / info_length) <= (1 - satisfied_pct);
}
