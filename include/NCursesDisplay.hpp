#pragma once

#include "Display.hpp"
#include <unordered_map>
#include <vector>
#include <mutex>

class NCursesDisplay : public Display {
public:
    NCursesDisplay();
    ~NCursesDisplay() override;

    void initialize_name(const std::string &name) override;
    void update_progress(const std::string &name, int progress) override;

    void render() override;
    void handle_input() override;

private:
    std::vector<std::string> names;
    std::unordered_map<std::string, int> progresses;
    std::mutex display_mutex;
    int num_progress = 0, max_name_length = 0;
    int num_row, num_col;
    int row_offset = 0, col_offset = 0;
    int pct_size = 5;
};
