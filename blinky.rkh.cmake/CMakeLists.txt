cmake_minimum_required(VERSION 3.16)
project(Blinky LANGUAGES C)

if (NOT DEFINED ENV{RKH_BASE})
    message(FATAL_ERROR "RKH_BASE is not defined in the environment. "
                        "Define it using 'export RKH_BASE=<path/to/RKH>', "
                        "before executing CMake")
endif()

# It includes external dependencies that provides CMake support like RKH 
# framework. These dependencies are implemented using git submodules
add_subdirectory(third-party)

# Application's source files should be defined in a specific subdirectory.
# The files in the application directory link RKH with the application. 
# This directory contains all application-specific files, such as 
# configuration options and source code.
add_executable(${PROJECT_NAME} 
               src/main.c 
               src/blinky.c 
               src/bsp_blinky.c 
               src/event.h
               src/priority.h
               src/signal.h
               src/blinky.h 
               src/rkhcfg.h
               src/bsp.h)

target_link_libraries(${PROJECT_NAME} PRIVATE rkh)
