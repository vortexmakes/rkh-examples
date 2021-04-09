cmake_minimum_required(VERSION 3.16)
project(Blinky LANGUAGES C)

if (NOT DEFINED ENV{RKH_BASE})
    message(FATAL_ERROR "RKH_BASE is not defined in the environment. "
                        "Define it using 'export RKH_BASE=<path/to/RKH>', "
                        "before executing CMake")
endif()

# Custom module and scripts
include($ENV{RKH_BASE}/cmake/boilerplate.cmake)

set(RKH_PLATFORM "Selects the RKH platform" CACHE STRING "__LNXGNU__")
option(RKH_DEV_BUILD 
       "Set to ON to enable RKH development on Linux platform" OFF)

if (RKH_DEV_BUILD)
    if (NOT RKH_PLATFORM STREQUAL "__LNXGNU__")
        message(FATAL_ERROR "Set RKH_PLATFORM to \"__LNXGNU__\" if you want to "
                "enable RKH_DEV_BUILD")
    else()
        set(DEV_BUILD ON CACHE BOOL "RKH development platform is enabled")
    endif()
endif()

find_package(rkh REQUIRED)

# Print variables and options
cmake_print_variables(RKH_BASE)
cmake_print_variables(RKH_PLATFORM)
cmake_print_variables(DEV_BUILD)
cmake_print_variables(NOT_SUBPROJECT)
cmake_print_variables(rkh_DIR)

# Application's source files should be defined in a specific subdirectory.
# The files in the application directory link RKH with the application. 
# This directory contains all application-specific files, such as 
# configuration options and source code.
add_executable(${PROJECT_NAME} 
               src/event.h
               src/priority.h
               src/signal.h
               src/blinky.h 
               src/rkhcfg.h
               src/main.c 
               src/blinky.c 
               src/bsp.h
               src/bsp_blinky.c)

# Library rkh::bsp will be linked automatically
target_link_libraries(${PROJECT_NAME} PRIVATE rkh::rkh)
