#include "display.hpp"
#include <algorithm>
#include <functional>
#include <thread>

#ifdef HAS_NCURSES
#include <ncurses.h>
#endif

using namespace std;

#ifdef HAS_NCURSES
const int UPDATE_DELAY_MS = 10;
#endif

void display(std::vector<SimProgress> &progresses) {
    #ifdef HAS_NCURSES
    initscr();
    nodelay(stdscr, true);
    keypad(stdscr, true);
    curs_set(0);
    int num_row, num_col;
    getmaxyx(stdscr, num_row, num_col);

    int row_offset = 0, col_offset = 0;
    int num_progress = static_cast<int>(progresses.size());
    int max_name_length = 0;
    int first_name_col = 3, last_name_col = num_col - 8;
    for (SimProgress &progress : progresses) {
        int name_length = static_cast<int>(progress.get_name().length());
        if (name_length > max_name_length) {
            max_name_length = name_length;
        }
    }

    while (any_of(progresses.begin(), progresses.end(), mem_fn(&SimProgress::working))) {
        erase();

        for (int i = 1; i < (num_row - 1); i++) {
            int prog_idx = i + row_offset;
            if (prog_idx >= num_progress) {
                break;
            }

            SimProgress &progress = progresses[prog_idx];

            string name = progress.get_name();
            size_t name_length = name.length();
            name = name.substr(col_offset, last_name_col - first_name_col);

            if (col_offset > 0) {
                mvaddstr(i, 0, "...");
            }

            if (name.length() < (name_length - col_offset)) {
                mvaddstr(i, last_name_col, "...");
            }

            mvaddstr(i, first_name_col, name.c_str());
            mvprintw(i, num_col - 5, "%3d%% ", progress.get_percentage());
        }

        if (row_offset > 0) {
            mvaddstr(0, 3, "...");
        }

        if (row_offset < (num_progress - num_row)) {
            mvaddstr(num_row - 1, 3, "...");
        }

        refresh();

        int ch = getch();
        if (ch == KEY_UP && row_offset > 0) {
            row_offset--;
        } else if (ch == KEY_DOWN && row_offset < (num_progress - num_row)) {
            row_offset++;
        } else if (ch == KEY_LEFT && col_offset > 0) {
            col_offset--;
        } else if (ch == KEY_RIGHT && col_offset <= (max_name_length - last_name_col)) {
            col_offset++;
        }

        this_thread::sleep_for(chrono::milliseconds(UPDATE_DELAY_MS));
    }

    endwin();
    #endif
}
