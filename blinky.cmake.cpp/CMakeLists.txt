cmake_minimum_required(VERSION 3.16)
project(blinky LANGUAGES C CXX)

set(RKH_CFG_FILE_PATH "src")

# Let's nicely support folders in IDEs
#set_property(GLOBAL PROPERTY USE_FOLDERS ON)
include(CMakePrintHelpers)

# This is here because preprocessor definitions specified with 
# add_definitions() are reproduced in Eclipse project file, but those 
# specified with target_compile_definitions() are not. So, if Eclipse 
# generator was not required, this definition would be dropped
add_definitions(-D${RKH_PLATFORM})   

# The compiled library code is here
add_subdirectory(third-party)

# The exucutable code is here
add_subdirectory(src)
