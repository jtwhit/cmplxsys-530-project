# Complex Systems 530 Project - Adaptive Search Engines
### By Jason Whitfield

## Code of Interest
Simulation code is contained in `distribution.cpp`, `SearchEngine.cpp`, `simulate.cpp`, `User.cpp`, and `WebPage.cpp`. The other files are display code and parameter sweep framework.

## Building and Running Project
Requires CMake version 3.5+ and a compiler with C++17 support.

1. Run CMake.

   `cmake .`

2. Run make.

   `make`

3. Execute program. `PARAM_PATH` is a path to a simulation parameter file or a directory containing such files. Results are written to the `outputs` directory.

   `./search_engine PARAM_PATH [PARAM_PATH ...]`

## Simulation Parameter Files
Simulation parameter files take the following form:
```
NUM_USERS
NUM_PAGES
MAX_INFO_VALUE
MIN_PAGE_LENGTH                     MAX_PAGE_LENGTH
MIN_PAGE_STANDARD_DEVIATION         MAX_PAGE_STANDARD_DEVIATION
CLICK_WEIGHT    INFO_FOUND_WEIGHT   TOPIC_SIMILARITY_WEIGHT
MIN_USER_INFO_LENGTH                MAX_USER_INFO_LENGTH
MIN_USER_INFO_STANDARD_DEVIATION    MAX_USER_INFO_STANDARD_DEVIATION
MIN_USER_SATISFACTION_RATIO         MAX_USER_SATISFACTION_RATIO
```

For example, default.txt:
```
3000
100000
10000
100 100
80 80
0.33333333333 0.33333333333 0.33333333333
50 50
10 10
0.95 0.95
```