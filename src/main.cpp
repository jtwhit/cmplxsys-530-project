#include "WebPage.hpp"
#include "User.hpp"
#include "distribution.hpp"
#include <iostream>
#include <algorithm>

using namespace std;

vector<WebPage> generate_pages(int num_pages) {
    vector<WebPage> pages;
    cout << "Creating web pages..." << endl;
    for (int i = 0; i < num_pages; i++) {
        pages.emplace_back(i, 1000, 20, 100);
    }
    
    return pages;
}

void iterate(const vector<WebPage> &pages) {
    User user(1000, 50, 0.9);

    try {
        while (!user.is_satisfied()) {
            const WebPage& page = user.choose_page(pages);
            user.read_page(page);
        }

        cout << "User read " << user.num_pages_read() << " pages." << endl;
    } catch (const UserError &e) {
        cout << "User Error: " << e.what() << endl;
    }
}

int main() {
    vector<WebPage> pages = generate_pages(100000);
    
    int i = 0;
    while (true) {
        cout << "Iteration " << i << endl;
        iterate(pages);
        i++;
    }
    
    return 0;
}
