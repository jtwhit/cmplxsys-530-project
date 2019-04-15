#include "WebPage.hpp"
#include <random>
#include <iostream>

using namespace std;

WebPage::WebPage(int id_, int max_info_int, int page_length, double page_std_dev) :
    id(id_) {
    random_device rd;
    default_random_engine random_gen(rd());

    uniform_int_distribution<> uniform(0, max_info_int);
    topic = uniform(random_gen);

    normal_distribution<> normal(topic, page_std_dev);
    while (static_cast<int>(data.size()) < page_length) {
        int info = static_cast<int>(round(normal(random_gen)));
        data.insert(info);
    }
}

int WebPage::get_id() const {
    return id;
}

int WebPage::get_topic() const {
    return topic;
}

const set<int>& WebPage::get_data() const {
    return data;
}
