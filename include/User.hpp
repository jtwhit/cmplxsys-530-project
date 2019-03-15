#pragma once

#include "WebPage.hpp"
#include <vector>
#include <set>
#include <string>

class User {
public:
    User(int max_info_int, int info_length, double sat_pct);

    void read_page(const WebPage &page);
    const WebPage& choose_page(const std::vector<WebPage> &pages) const;
    bool is_satisfied() const;

    int num_pages_read() const;

private:
    double score_title_relevancy(const WebPage &page) const;

    double satisfied_pct;
    int topic, original_info_length;
    std::set<int> search_info, read_pages;
};

class UserError {
public:
    UserError(const std::string &error_msg);
    std::string what() const;

private:
    std::string msg;
};

