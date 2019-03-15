#include "User.hpp"
#include "distribution.hpp"
#include <iostream>

using namespace std;

User::User(int max_info_int, int info_length, double sat_pct) :
    satisfied_pct(sat_pct),
    original_info_length(info_length) {
    topic = uniform_int(0, max_info_int);
    vector<int> info = normal_ints(info_length, topic, 10);
    search_info = set<int>(info.begin(), info.end());
}

void User::read_page(const WebPage &page) {
    for (int info : page.get_data()) {
        search_info.erase(info);
    }
    read_pages.insert(page);
}

const WebPage& User::choose_page(const vector<WebPage> &pages) const {
    for (const WebPage& page : pages) {
        if (read_pages.count(page) == 0) {
            return page;
        }
    }

    throw UserError("Already read all web pages.");
}

bool User::is_satisfied() const {
    return (static_cast<double>(search_info.size()) / original_info_length) <= (1 - satisfied_pct);
}

int User::num_pages_read() const {
    return read_pages.size();
}

UserError::UserError(const string &error_msg) :
    msg(error_msg) {}

string UserError::what() const {
    return msg;
}
