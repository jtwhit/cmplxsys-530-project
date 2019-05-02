#pragma once

#include "Run.hpp"
#include <string>

class SweepRun : public Run {
public:
    SweepRun(const std::string &name_, const std::string &path_);

    void execute() override;

private:
    std::string name, path;
};
