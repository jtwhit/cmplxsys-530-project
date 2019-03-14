cmake_minimum_required(VERSION 3.5)

project(search_engine)

include_directories(include)

file(GLOB SOURCES "src/*.cpp")

add_executable(search_engine ${SOURCES})

target_compile_options(search_engine PUBLIC -O3 -std=c++11 -Wall -Werror)
