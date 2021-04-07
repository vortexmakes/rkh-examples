cmake_minimum_required(VERSION 3.16)
project(Blinky LANGUAGES C)

if (NOT DEFINED ENV{RKH_BASE})
    message(FATAL_ERROR "RKH_BASE is not defined in the environment. "
                        "Define it using 'export RKH_BASE=<path/to/RKH>', "
                        "before executing CMake")
endif()

# Custom module and scripts
include($ENV{RKH_BASE}/cmake/boilerplate.cmake)

add_subdirectory(${RKH_BASE} ${CMAKE_CURRENT_BINARY_DIR}/rkh)
add_subdirectory(${RKH_BASE}/demo/libbsp ${CMAKE_CURRENT_BINARY_DIR}/bsp)

# Application's source files should be defined in a specific subdirectory.
# The files in the application directory link RKH with the application. 
# This directory contains all application-specific files, such as 
# configuration options and source code.
add_executable(${PROJECT_NAME} 
               src/main.c 
               src/blinky.c 
               ${RKH_BASE}/demo/libbsp/platform/80x86/linux/bsp_blinky.c 
               src/event.h
               src/priority.h
               src/signal.h
               src/blinky.h 
               src/rkhcfg.h
               src/bsp.h)

target_link_libraries(${PROJECT_NAME} PRIVATE rkh)
target_link_libraries(${PROJECT_NAME} PRIVATE bsp)
