cmake_minimum_required(VERSION 2.6)
project(ConsoleGameEngine)

find_package(Curses REQUIRED)
include_directories(${CURSES_INCLUDE_DIRS})

include_directories(include)
file(GLOB SOURCES "src/*.cpp")

add_executable(CGE ${SOURCES})

set(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} "-std=c++17  -pthread")
#set_property(TARGET CGE PROPERTY CXX_STANDARD 17 -pthread)
target_link_libraries(CGE ${CURSES_LIBRARIES})
# TODO: Run -Werror in CI.
target_compile_options(CGE PRIVATE -Wall -Wextra)
