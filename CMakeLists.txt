cmake_minimum_required(VERSION 3.5)

project(search_engine)

include_directories(include)

add_executable(search_engine "src/main.cpp")
target_sources(search_engine PRIVATE
               "src/RunQueue.cpp"
               "src/distribution.cpp"
               "src/SingleRun.cpp"
               "src/SweepRun.cpp"
               "src/SearchEngine.cpp"
               "src/SimParams.cpp"
               "src/simulate.cpp"
               "src/User.cpp"
               "src/WebPage.cpp")

add_executable(ants "src/ANTs.cpp")
target_sources(ants PRIVATE
               "src/distribution.cpp"
               "src/SearchEngine.cpp"
               "src/SimParams.cpp"
               "src/simulate.cpp"
               "src/User.cpp"
               "src/WebPage.cpp")

find_package(Threads REQUIRED)

target_link_libraries(search_engine ${CMAKE_THREAD_LIBS_INIT} -lstdc++fs)

target_compile_options(search_engine PUBLIC -O3 -std=c++17 -Wall -Werror)

target_link_libraries(ants ${CMAKE_THREAD_LIBS_INIT})

target_compile_options(ants PUBLIC -O3 -std=c++17 -Wall -Werror)
