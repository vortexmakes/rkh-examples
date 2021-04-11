cmake_minimum_required(VERSION 3.16)
project(Blinky LANGUAGES C)

find_package(rkh 3.2.3 REQUIRED)

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

# Library rkh::bsp for Linux GNU platform will be linked automatically
target_link_libraries(${PROJECT_NAME} PRIVATE rkh::rkh)
