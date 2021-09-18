cmake_minimum_required(VERSION 3.13)  # CMake version check
project(philosophers)               # Create project "simple_example"
set(CMAKE_CXX_STANDARD 14)            # Enable c++14 standard

# Add main.cpp file of project root directory as source file
set(SOURCE_FILES srcs/main.c includes/sauce.h srcs/philosopher_actions.c srcs/utilities.c srcs/conditions.c srcs/parsing.c srcs/feast.c)

# Add executable target with source files listed in SOURCE_FILES variable
add_executable(main ${SOURCE_FILES})