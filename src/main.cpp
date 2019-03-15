#include "WebPage.hpp"
#include "User.hpp"
#include "distribution.hpp"
#include <iostream>

using namespace std;

const int NUM_PAGES = 1000000;

const int MAX_INFO_NUM = 10000;

const int PAGE_TITLE_LENGTH = 20;
const int PAGE_DATA_LENGTH = 100;

const int USER_INFO_LENGTH = 50;
const double USER_SATISFACTION_PCT = 0.95;

vector<WebPage> generate_pages(int num_pages) {
    vector<WebPage> pages;
    cout << "Creating web pages..." << endl;
    for (int i = 0; i < num_pages; i++) {
        pages.emplace_back(i, MAX_INFO_NUM, PAGE_TITLE_LENGTH, PAGE_DATA_LENGTH);
    }
    
    return pages;
}

int iterate(const vector<WebPage> &pages) {
    User user(MAX_INFO_NUM, USER_INFO_LENGTH, USER_SATISFACTION_PCT);

    try {
        while (!user.is_satisfied()) {
            const WebPage& page = user.choose_page(pages);
            user.read_page(page);
        }
    } catch (const UserError &e) {
        cout << "User Error: " << e.what() << endl;
    }

    return user.num_pages_read();
}

int main() {
    vector<WebPage> pages = generate_pages(NUM_PAGES);
    
    int total_sum = 0;
    int i = 0;
    while (true) {
        total_sum += iterate(pages);
        i++;
        cout << "Iteration " << i << ": " << static_cast<double>(total_sum) / i << endl;
    }
    
    return 0;
}
