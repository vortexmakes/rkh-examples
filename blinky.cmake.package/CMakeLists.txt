cmake_minimum_required(VERSION 3.16)
project(Blinky LANGUAGES C)

# When using find_package() the configuration file (rkhcfg.h) is taken from 
# the installation directory where is located the library. If the 
# application wants to use its own configuration file, the library must be 
# compiled and reinstalled.
find_package(rkh 3.3.00 REQUIRED)

# Application's source files should be defined in a specific subdirectory.
# The files in the application directory link RKH with the application. 
# This directory contains all application-specific files, such as 
# configuration options and source code.
add_executable(${PROJECT_NAME} 
               src/event.h
               src/priority.h
               src/signal.h
               src/blinky.h 
               src/main.c 
               src/blinky.c 
               src/bsp.h
               src/bsp_blinky.c)

# Library rkh::bsp for Linux GNU platform will be linked automatically
target_link_libraries(${PROJECT_NAME} PRIVATE rkh::rkh)
