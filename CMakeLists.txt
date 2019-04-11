cmake_minimum_required(VERSION 3.5)

project(search_engine)

include_directories(include)

file(GLOB SOURCES "src/*.cpp")

find_package(Threads)

add_executable(search_engine ${SOURCES})

target_link_libraries(search_engine ${CMAKE_THREAD_LIBS_INIT} -lstdc++fs)

target_compile_options(search_engine PUBLIC -O3 -std=c++17 -Wall -Werror)
