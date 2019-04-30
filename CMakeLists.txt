cmake_minimum_required(VERSION 3.5)

project(search_engine)

include_directories(include)

add_executable(search_engine "src/main.cpp")
target_sources(search_engine PRIVATE
               "src/distribution.cpp"
               "src/PathQueue.cpp"
               "src/SearchEngine.cpp"
               "src/simulate.cpp"
               "src/User.cpp"
               "src/WebPage.cpp")

add_executable(ants "src/ANTs.cpp")
target_sources(ants PRIVATE
               "src/distribution.cpp"
               "src/SearchEngine.cpp"
               "src/simulate.cpp"
               "src/User.cpp"
               "src/WebPage.cpp")

find_package(Threads REQUIRED)

find_package(Curses)
if(CURSES_HAVE_NCURSES_H)
    add_definitions(-DHAS_NCURSES)
    include_directories(${CURSES_INCLUDE_DIR})
    target_sources(search_engine PRIVATE "src/NCursesDisplay.cpp")
else()
    target_sources(search_engine PRIVATE "src/ConsoleDisplay.cpp")
endif()

target_link_libraries(search_engine ${CMAKE_THREAD_LIBS_INIT} -lstdc++fs ${CURSES_LIBRARIES} -lncurses)

target_compile_options(search_engine PUBLIC -O3 -std=c++17 -Wall -Werror)

target_link_libraries(ants ${CMAKE_THREAD_LIBS_INIT} -lstdc++fs ${CURSES_LIBRARIES} -lncurses)

target_compile_options(ants PUBLIC -O3 -std=c++17 -Wall -Werror)
