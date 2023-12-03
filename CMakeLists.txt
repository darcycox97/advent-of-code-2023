cmake_minimum_required(VERSION 3.5.0)
project(aoc_2023)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# all cc files without having to manually add each one
file(GLOB_RECURSE SOURCE_FILES "*.cc")

add_executable(aoc ${SOURCE_FILES})