#include "WebPage.hpp"
#include "distribution.hpp"
#include <algorithm>

using namespace std;

WebPage::WebPage(int max_info_int, int title_length, int page_length) {
    topic = uniform_int(0, max_info_int);
    data = normal_ints(page_length, topic, 10);
    copy_n(data.begin(), title_length, back_inserter(title));
    shuffle(data.begin(), data.end(), random_gen);
}

bool WebPage::operator<(const WebPage &other) {
    return topic < other.topic;
}
