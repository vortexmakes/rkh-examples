cmake_minimum_required(VERSION 3.16)
project(Blinky LANGUAGES C CXX)

if (NOT DEFINED ENV{RKH_BASE})
    message(FATAL_ERROR "RKH_BASE is not defined in the environment. "
                        "Define it using 'export RKH_BASE=<path/to/RKH>', "
                        "before executing CMake")
endif()

# This is here because preprocessor definitions specified with 
# add_definitions() are reproduced in Eclipse project file, but those 
# specified with target_compile_definitions() are not. So, if Eclipse 
# generator was not required, this definition would be dropped
add_definitions(-D${RKH_PLATFORM})   

# It includes external dependencies that provide CMake support like the RKH 
# framework. These dependencies are managed using git submodules
add_subdirectory(third-party)

# Application's source files should be defined in a specific subdirectory.
# The files in the application directory link RKH with the application. 
# This directory contains all application-specific files, such as 
# configuration options and source code.
add_executable(${PROJECT_NAME} 
               src/main.cpp 
               src/blinky.cpp 
               src/bsp_blinky.cpp 
               src/blinky.h 
               src/rkhcfg.h
               src/signal.h
               src/event.h
               src/priority.h
               src/bsp.h)

target_link_libraries(${PROJECT_NAME} PRIVATE rkh)
