#include "WebPage.hpp"
#include "distribution.hpp"
#include <iostream>
#include <algorithm>

using namespace std;

int main() {
    vector<WebPage> pages;

    cout << "Creating web pages..." << endl;
    for (int i = 0; i < 100000; i++) {
        pages.emplace_back(200, 20, 100);
    }
    
    int i = 0;
    while (true) {
        cout << "Iteration " << i << endl;
        sort(pages.begin(), pages.end());
        shuffle(pages.begin(), pages.end(), random_gen);
        i++;
    }
    
    return 0;
}
