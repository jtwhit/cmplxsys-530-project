#include "ConsoleDisplay.hpp"
#include <iostream>

using namespace std;

void ConsoleDisplay::update_progress(const string &name, int progress) {
    lock_guard<mutex> display_lock(display_mutex);

    if (running.count(name) == 0) {
        started.insert(name);
        running.insert(name);
    } else if (progress == 100 && running.count(name) > 0) {
        running.erase(name);
        ended.insert(name);
    }
}

void ConsoleDisplay::render() {
    lock_guard<mutex> display_lock(display_mutex);

    for (const string &name : started) {
        cout << "Running " << name << "..." << endl;
    }
    started.clear();

    for (const string &name : ended) {
        cout << "Finished " << name << "." << endl;
    }
    ended.clear();
}
