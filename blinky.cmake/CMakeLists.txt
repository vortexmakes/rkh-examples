cmake_minimum_required(VERSION 3.16)
project(blinky.cmake LANGUAGES C)

set(RKH_CFG_FILE_PATH "src")
include(CMakePrintHelpers)

# The compiled library code is here
add_subdirectory(third-party)

# The exucutable code is here
add_subdirectory(src)
