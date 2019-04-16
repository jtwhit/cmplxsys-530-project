#include "User.hpp"
#include "distribution.hpp"
#include <algorithm>
#include <cmath>
#include <numeric>
#include <tuple>

using namespace std;

User::User(int max_info_int, int info_length, double info_std_dev, double sat_pct) :
    satisfied_pct(sat_pct),
    original_info_length(info_length) {
    tie(topic, search_info) = generate_info(0, max_info_int - 1, info_length, info_std_dev);
}

ActionData User::read_page(double query, const WebPage &page) {
    set<int> new_search_info;
    set<int> page_data = page.get_data();
    set_difference(search_info.begin(), search_info.end(),
                   page_data.begin(), page_data.end(),
                   inserter(new_search_info, new_search_info.begin()));

    int num_found = search_info.size() - new_search_info.size();
    search_info = new_search_info;
    read_pages.insert(page.get_id());

    ActionData data;
    data.page_id = page.get_id();
    data.query = query;
    data.info_found = num_found;
    data.topic_similarity = abs(query - page.get_topic());

    return data;
}

int User::choose_page(double query, const vector<WebPage> &pages) const {
    double max_score = 0;
    int max_index = -1;
    for (size_t i = 0; i < pages.size(); i++) {
        const WebPage& page = pages[i];
        if (read_pages.count(page.get_id()) != 0) {
            continue;
        }
        
        int page_rank = (i + 1);
        if ((1.0 / page_rank) < max_score && max_index >= 0) {
            break;
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
    return (static_cast<double>(search_info.size()) / original_info_length) <= (1 - satisfied_pct);
}
