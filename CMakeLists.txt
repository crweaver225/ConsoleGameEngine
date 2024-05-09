cmake_minimum_required(VERSION 3.12) # Use a more recent version of CMake
project(ConsoleGameEngine)

# Set the C++ standard for the project
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Set the target architecture if necessary (comment out if not needed)
set(CMAKE_OSX_ARCHITECTURES "arm64")

# Adjust the path as needed. This is just for my local machine
set(LIBRARY_PATH "$ENV{HOME}/Desktop/cpp-playground/Libraries") 

# Find packages
find_package(Curses REQUIRED)
find_package(Torch REQUIRED PATHS "${LIBRARY_PATH}/libtorch")
find_package(Threads REQUIRED)

# Include directories
include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CURSES_INCLUDE_DIRS}
    ${TORCH_INCLUDE_DIRS}
)

# My Mac was missing this package
find_package(OpenMP)
if(OpenMP_CXX_FOUND)
    message(STATUS "OpenMP found")
else()
    message(STATUS "OpenMP not found")
    set(LIBOMP_PATH ${LIBRARY_PATH})
    set(RPATH ${LIBOMP_PATH})
    list(APPEND CMAKE_BUILD_RPATH ${RPATH})
endif()

# Define the sources
file(GLOB SOURCES "src/*.cpp")

# Define the executable target
add_executable(CGE ${SOURCES})

# Compiler options
target_compile_options(CGE PRIVATE -Wall -Wextra -pthread)

# Link libraries
target_link_libraries(CGE
    PRIVATE
    ${CURSES_LIBRARIES}
    ${TORCH_LIBRARIES}
    ${CMAKE_THREAD_LIBS_INIT}
)
