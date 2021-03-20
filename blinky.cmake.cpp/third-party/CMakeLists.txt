find_package(Git QUIET)
if(GIT_FOUND AND 
   EXISTS "${PROJECT_SOURCE_DIR}/../.git" AND 
   EXISTS "${PROJECT_SOURCE_DIR}/../.gitmodules")
    # Update submodules as needed
    option(GIT_SUBMODULE "Check submodules during build" ON)
    if(GIT_SUBMODULE)
        set(SUBMODULE "RKH")
        message(STATUS "${SUBMODULE} submodule update")
        execute_process(COMMAND ${GIT_EXECUTABLE} submodule update --init ${SUBMODULE}
                        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
                        RESULT_VARIABLE GIT_SUBMOD_RESULT)
        if(NOT GIT_SUBMOD_RESULT EQUAL "0")
            message(FATAL_ERROR "git submodule update failed with ${GIT_SUBMOD_RESULT}")
        endif()
    endif()
else()
    cmake_print_variables(GIT_FOUND GIT_SUBMODULE PROJECT_SOURCE_DIR CMAKE_CURRENT_SOURCE_DIR)
endif()

if (EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/RKH")
    # The RKH framework port for Linux platform requires pthread library
    set(CMAKE_THREAD_PREFER_PTHREAD ON)
    set(THREADS_PREFER_PTHREAD_FLAG ON)
    find_package(Threads REQUIRED)

    add_library(rkh STATIC 
                RKH/source/fwk/src/rkhfwk_bittbl.c
                RKH/source/fwk/src/rkhfwk_dynevt.c
                RKH/source/fwk/src/rkhfwk_evtpool.c
                RKH/source/fwk/src/rkhfwk_pubsub.c
                RKH/source/fwk/src/rkhfwk_rdygrp.c
                RKH/source/fwk/src/rkhfwk_sched.c
                RKH/source/fwk/src/rkhfwk_version.c
                RKH/source/mempool/src/rkhmempool.c
                RKH/source/portable/80x86/linux_st/gnu/rkhport.c
                RKH/source/queue/src/rkhqueue.c
                RKH/source/sm/src/rkhsm.c
                RKH/source/sma/src/rkhsma.c
                RKH/source/sma/src/rkhsma_prio.c
                RKH/source/sma/src/rkhsma_sync.c
                RKH/source/tmr/src/rkhtmr.c
                RKH/source/trc/src/rkhtrc_filter.c
                RKH/source/trc/src/rkhtrc_record.c
                RKH/source/trc/src/rkhtrc_stream.c
                RKH/demo/libbsp/platform/80x86/linux/assert.c
                RKH/demo/libbsp/platform/80x86/linux/getopt.c
                RKH/demo/libbsp/platform/80x86/linux/hook.c
                RKH/demo/libbsp/platform/80x86/linux/trace_io.c
                RKH/demo/libbsp/platform/80x86/linux/trace_io_tcp.c)
    target_include_directories(rkh PUBLIC 
                               RKH/source/portable/80x86/linux_st/gnu
                               RKH/demo/libbsp/platform/80x86/linux
                               RKH/demo/libbsp/common
                               RKH/source/fwk/inc
                               RKH/source/mempool/inc
                               RKH/source/queue/inc
                               RKH/source/sm/inc
                               RKH/source/sma/inc
                               RKH/source/tmr/inc
                               RKH/source/trc/inc
                               ${CMAKE_SOURCE_DIR}/${RKH_CFG_FILE_PATH})
    target_compile_definitions(rkh PUBLIC ${RKH_PLATFORM})
    target_link_libraries(rkh PRIVATE Threads::Threads)
else()
    message(FATAL_ERROR "RKH do not found in ${CMAKE_CURRENT_SOURCE_DIR}")
endif()
