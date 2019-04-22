#include "NCursesDisplay.hpp"
#include <algorithm>
#include <functional>
#include <thread>
#include <ncurses.h>

using namespace std;

NCursesDisplay::NCursesDisplay() {
    initscr();
    nodelay(stdscr, true);
    keypad(stdscr, true);
    curs_set(0);
    getmaxyx(stdscr, num_row, num_col);
}

NCursesDisplay::~NCursesDisplay() {
    endwin();
}

void NCursesDisplay::initialize_name(const string &name) {
    lock_guard<mutex> display_lock(display_mutex);
    num_progress++;
    names.push_back(name);
    progresses[name] = 0;
    if (max_name_length < static_cast<int>(name.length())) {
        max_name_length = name.length();
    }
}

void NCursesDisplay::update_progress(const string &name, int progress) {
    lock_guard<mutex> display_lock(display_mutex);
    if (progresses.find(name) != progresses.end()) {
        progresses[name] = progress;
    }
}

void NCursesDisplay::render() {
    lock_guard<mutex> display_lock(display_mutex);
    erase();

    for (int i = 0; i < num_row; i++) {
        int prog_idx = i + row_offset;
        if (prog_idx >= num_progress) {
            break;
        }

        int pct_col = num_col - pct_size;
        string name = names[prog_idx];

        if (col_offset < static_cast<int>(name.length())) {
            string trunc_name = name.substr(col_offset, pct_col - 1);
            mvaddstr(i, 0, trunc_name.c_str());
        }
        mvprintw(i, pct_col, "%3d%% ", progresses[name]);
    }

    refresh();
}

void NCursesDisplay::handle_input() {
    lock_guard<mutex> display_lock(display_mutex);
    int ch = getch();
    if (ch == KEY_UP) {
        row_offset--;
    } else if (ch == KEY_DOWN) {
        row_offset++;
    } else if (ch == KEY_LEFT) {
        col_offset--;
    } else if (ch == KEY_RIGHT) {
        col_offset++;
    }

    getmaxyx(stdscr, num_row, num_col);

    int max_row = num_progress - num_row;
    if (row_offset > max_row) {
        row_offset = max_row;
    }
    if (row_offset < 0) {
        row_offset = 0;
    }

    int max_col = max_name_length - (num_col - pct_size + 1);
    if (col_offset > max_col) {
        col_offset = max_col;
    }
    if (col_offset < 0) {
        col_offset = 0;
    }
}
