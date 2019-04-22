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
    first_name_col = 3;
    last_name_col = num_col - 8;
}

NCursesDisplay::~NCursesDisplay() {
    endwin();
}

void NCursesDisplay::initialize_name(const string &name) {
    lock_guard<mutex> display_lock(display_mutex);
    num_progress++;
    names.push_back(name);
    progresses[name] = 0;
    int name_length = static_cast<int>(name.length());
    if (name_length > max_name_length) {
        max_name_length = name_length;
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

    for (int i = 1; i < (num_row - 1); i++) {
        int prog_idx = i + row_offset - 1;
        if (prog_idx >= num_progress) {
            break;
        }

        string name = names[prog_idx];
        string trunc_name = name.substr(col_offset, last_name_col - first_name_col);

        if (col_offset > 0) {
            mvaddstr(i, 0, "...");
        }

        if (trunc_name.length() < (trunc_name.length() - col_offset)) {
            mvaddstr(i, last_name_col, "...");
        }

        mvaddstr(i, first_name_col, trunc_name.c_str());
        mvprintw(i, num_col - 5, "%3d%% ", progresses[name]);
    }

    if (row_offset > 0) {
        mvaddstr(0, 3, "...");
    }

    if (row_offset < (num_progress - num_row)) {
        mvaddstr(num_row - 1, 3, "...");
    }

    refresh();
}

void NCursesDisplay::handle_input() {
    lock_guard<mutex> display_lock(display_mutex);
    int ch = getch();
    if (ch == KEY_UP && row_offset > 0) {
        row_offset--;
    } else if (ch == KEY_DOWN && row_offset <= (num_progress - num_row)) {
        row_offset++;
    } else if (ch == KEY_LEFT && col_offset > 0) {
        col_offset--;
    } else if (ch == KEY_RIGHT && col_offset <= (max_name_length - last_name_col)) {
        col_offset++;
    }
}
