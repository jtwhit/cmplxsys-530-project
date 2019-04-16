#pragma once

#include <set>

class WebPage {
public:
    WebPage(int id_, int max_info_int, int page_length, double page_std_dev);

    int get_id() const;
    int get_topic() const;
    const std::set<int>& get_data() const;

private:
    int id, topic;
    std::set<int> data;
};
