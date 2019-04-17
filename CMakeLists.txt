cmake_minimum_required(VERSION 3.5)

project(search_engine)

include_directories(include)

file(GLOB SOURCES "src/*.cpp")

find_package(Threads REQUIRED)

find_package(Curses)
if(CURSES_HAVE_NCURSES_H)
    add_definitions(-DHAS_NCURSES)
    include_directories(${CURSES_INCLUDE_DIR})
endif()

add_executable(search_engine ${SOURCES})

target_link_libraries(search_engine ${CMAKE_THREAD_LIBS_INIT} -lstdc++fs ${CURSES_LIBRARIES} -lncurses)

target_compile_options(search_engine PUBLIC -O3 -std=c++17 -Wall -Werror)
