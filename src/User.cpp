#include "User.hpp"
#include "distribution.hpp"
#include <numeric>
#include <cmath>
#include <algorithm>
#include <limits>
#include <iostream>

using namespace std;

User::User(int max_info_int, int info_length, double sat_pct) :
    satisfied_pct(sat_pct),
    original_info_length(info_length) {
    topic = uniform_int(0, max_info_int);
    vector<int> info = normal_ints(info_length, topic, 50);
    search_info = set<int>(info.begin(), info.end());
}

void User::read_page(const WebPage &page) {
    for (int info : page.get_data()) {
        search_info.erase(info);
    }
    read_pages.insert(page.get_id());
}

const WebPage& User::choose_page(const vector<WebPage> &pages) const {
    double max_score = numeric_limits<double>::lowest();
    int max_index = -1;
    for (size_t i = 0; i < pages.size(); i++) {
        const WebPage& page = pages[i];
        if (read_pages.count(page.get_id()) != 0)
            continue;
        
        double title_relevancy = score_title_relevancy(page);
        double total_score = title_relevancy * (i + 1);
        if (total_score > max_score) {
            max_score = total_score;
            max_index = i;
        }
    }

    if (max_index >= 0) {
        return pages[max_index];
    }

    throw UserError("Already read all web pages.");
}

bool User::is_satisfied() const {
    return (static_cast<double>(search_info.size()) / original_info_length) <= (1 - satisfied_pct);
}

int User::num_pages_read() const {
    return read_pages.size();
}

double User::score_title_relevancy(const WebPage &page) const {
    const vector<int> &title = page.get_title();
    int title_sum = accumulate(title.begin(), title.end(), 0);
    double title_avg = static_cast<double>(title_sum) / title.size();

    return -abs(topic - title_avg);
}

UserError::UserError(const string &error_msg) :
    msg(error_msg) {}

string UserError::what() const {
    return msg;
}
