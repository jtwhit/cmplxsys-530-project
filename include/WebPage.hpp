#pragma once

#include <vector>

class WebPage {
public:
    WebPage(int page_id, int max_info_int, int title_length, int page_length);

    const int get_id() const;
    const std::vector<int>& get_title() const;
    const std::vector<int>& get_data() const;

private:
    int id, topic;
    std::vector<int> data, title;
};
