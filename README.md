# Complex Systems 530 Project - Adaptive Search Engines
### By Jason Whitfield

## Building and Running Project
Requires CMake version 3.5+ and a compiler with C++17 support.

1. Run CMake.

   `cmake .`

2. Run make.

   `make`

3. Execute program. `PARAM_PATH` is a path to a simulation parameter file or a directory containing such files. Results are written to the "outputs" directory. If only one path is given, the simulation will print its progress.

   `./search_engine PARAM_PATH [PARAM_PATH ...]`

## Simulation Parameter Files
Simulation parameter files take the following form:
```
NUM_USERS
NUM_PAGES
MAX_INFO_VALUE
PAGE_LENGTH
PAGE_STANDARD_DEVIATION
CLICK_WEIGHT INFO_FOUND_WEIGHT TOPIC_SIMILARITY_WEIGHT
USER_INFO_LENGTH
USER_INFO_STANDARD_DEVIATION
USER_SATISFACTION_RATIO
```

For example, default.txt:
```
3000
100000
10000
100
80
0.33333333333 0.33333333333 0.33333333333
50
10
0.95
```