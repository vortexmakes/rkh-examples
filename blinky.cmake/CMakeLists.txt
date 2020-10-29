cmake_minimum_required(VERSION 3.16)
project(blinky LANGUAGES C)

set(RKH_CFG_FILE_PATH "src")

# Let's nicely support folders in IDEs
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

include(CMakePrintHelpers)
cmake_print_variables(CMAKE_ECLIPSE_VERSION)

# The compiled library code is here
add_subdirectory(third-party)

# The exucutable code is here
add_subdirectory(src)
