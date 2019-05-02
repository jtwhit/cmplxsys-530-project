#pragma once

#include "Run.hpp"

#include <string>

class SingleRun : public Run {
public:
    SingleRun(const std::string &name_, const std::string &path_);

    void execute() override;

private:
    std::string name, path;
};
