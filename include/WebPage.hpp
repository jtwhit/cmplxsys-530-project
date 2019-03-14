#pragma once

#include <vector>

class WebPage {
public:
    WebPage(int max_info_int, int title_length, int page_length);
    bool operator<(const WebPage &other);

private:
    int topic;
    std::vector<int> data, title;
};
