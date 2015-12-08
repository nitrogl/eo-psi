# Main cmake file.
set (LIBRARY_NAME "hasbuckets")

# Set the makefile verbose
set (CMAKE_VERBOSE_MAKEFILE 1)

project(${LIBRARY_NAME})

cmake_minimum_required(VERSION 2.8)

# Compiler flags
set (OPTIMIZATION "2")
# set (DEBUG_INFO_FLAG "-g")
set (PLATFORM_INDEPENDENT_CODE_FLAG "-fPIC")
set (CXX_STD "-std=c++11")

set (CMAKE_CXX_FLAGS "-Wall -Wextra -O${OPTIMIZATION} ${PLATFORM_INDEPENDENT_CODE_FLAG} ${DEBUG_FLAG} ${DEBUG_INFO_FLAG} ${CXX_STD}")


add_subdirectory(src/)