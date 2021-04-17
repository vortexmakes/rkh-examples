find_package(Git QUIET)

# Update submodules
if(GIT_FOUND AND 
   EXISTS "${PROJECT_SOURCE_DIR}/../.git" AND 
   EXISTS "${PROJECT_SOURCE_DIR}/../.gitmodules")
    # Update submodules as needed
    option(GIT_SUBMODULE "Check submodules during build" OFF)
    if(GIT_SUBMODULE)
        set(SUBMODULE RKH)    # Submodule list 
        foreach(submodule ${SUBMODULE})
            message(STATUS "${submodule} submodule update")
            execute_process(COMMAND ${GIT_EXECUTABLE} 
                            submodule update --init ${submodule}
                            WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
                            RESULT_VARIABLE GIT_SUBMOD_RESULT)
            if(NOT GIT_SUBMOD_RESULT EQUAL "0")
                message(FATAL_ERROR 
                        "git submodule update failed with 
                        ${GIT_SUBMOD_RESULT}")
            endif()
            # --------- It is used only for testing purposes ---------
            # It changes to branch 'issue-cmake' while it is developed
            execute_process(COMMAND ${GIT_EXECUTABLE}
                            checkout issue-cmake
                            WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/RKH
                            RESULT_VARIABLE GIT_SUBMOD_RESULT)
            if(NOT GIT_SUBMOD_RESULT EQUAL "0")
                message(FATAL_ERROR 
                        "git checkout issue-cmake failed with 
                        ${GIT_SUBMOD_RESULT}")
            endif()
            # --------------------------------------------------------
        endforeach()
    endif()
else()
    cmake_print_variables(GIT_FOUND GIT_SUBMODULE PROJECT_SOURCE_DIR CMAKE_CURRENT_SOURCE_DIR)
endif()

# Bundle RKH framework and Blinky application
if (EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/RKH)
    # Custom module and scripts
    include(${CMAKE_CURRENT_SOURCE_DIR}/RKH/cmake/boilerplate.cmake)
    add_subdirectory(RKH)
else()
    message(FATAL_ERROR "RKH do not found in ${CMAKE_CURRENT_SOURCE_DIR}")
endif()
