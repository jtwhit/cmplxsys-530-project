# Complex Systems 530 Project - Adaptive Search Engines
### By Jason Whitfield

## Code of Interest
Simulation and ANTs code is contained in `ANTS.cpp`, `distribution.cpp`, `SearchEngine.cpp`, `simulate.cpp`, `User.cpp`, and `WebPage.cpp`. The other files are just for running the simulations efficiently.

## Building and Running Project
Requires CMake version 3.5+ and a compiler with C++17 support.

1. Run CMake.

   `cmake .`

2. Run make.

   `make`

3. Execute simulation program. `PARAM_PATH` is a path to a simulation parameter file or a directory containing such files. Results are written to the `outputs` directory.

   `./search_engine PARAM_PATH [PARAM_PATH ...]`

4. Execute ANTs optimization program.

   `./ants`

## Simulation Parameter Files
Simulation parameter either specify the parameters used to run the simulation, or a sweep on one parameter. Unspecified parameters take the value in params/default.params.

Valid parameter settings:
```
NUM_USERS
NUM_PAGES
MAX_INFO_INT
PAGE_LENGTH
PAGE_LENGTH_MIN
PAGE_LENGTH_MAX
PAGE_STD_DEV
PAGE_STD_DEV_MIN
PAGE_STD_DEV_MAX
PAGE_CLICK_WEIGHT
INFO_FOUND_WEIGHT
TOPIC_SIMILARITY_WEIGHT
USER_LENGTH
USER_LENGTH_MIN
USER_LENGTH_MAX
USER_STD_DEV
USER_STD_DEV_MIN
USER_STD_DEV_MAX
USER_SAT_PCT
USER_SAT_PCT_MIN
USER_SAT_PCT_MAX
```
