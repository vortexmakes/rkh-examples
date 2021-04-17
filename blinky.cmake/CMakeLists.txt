cmake_minimum_required(VERSION 3.16)
project(Blinky LANGUAGES C)

if (NOT DEFINED ENV{RKH_BASE})
    message(FATAL_ERROR "RKH_BASE is not defined in the environment. "
                        "Define it using 'export RKH_BASE=<path/to/RKH>', "
                        "before executing CMake")
endif()

# It includes external dependencies that provide CMake support like the RKH 
# framework. These dependencies are managed using git submodules
add_subdirectory(third-party)

# Application's source files should be defined in a specific subdirectory.
# The files in the application directory link RKH with the application. 
# This directory contains all application-specific files, such as 
# configuration options and source code.
add_executable(${PROJECT_NAME} 
    src/main.c 
    src/blinky.c
    ${CACHED_RKH_BASE}/demo/libbsp/platform/80x86/linux/bsp_blinky.c)

target_include_directories(${PROJECT_NAME} PRIVATE 
                           ${CMAKE_CURRENT_SOURCE_DIR}/src)

target_link_libraries(${PROJECT_NAME} PRIVATE rkh)
