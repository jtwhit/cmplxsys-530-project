#pragma once

#include "SearchEngine.hpp"
#include "WebPage.hpp"
#include <set>
#include <string>
#include <vector>

class User {
public:
    User(int max_info_int, int info_length, double info_std_dev, double sat_pct);

    ActionData read_page(double query, const WebPage &page);
    int choose_page(double query, SearchEngine &search_engine) const;
    double generate_query() const;
    bool is_satisfied() const;

private:
    double satisfied_pct;
    int topic, original_info_length, max_depth = 0;
    std::set<int> search_info, read_pages;
};
