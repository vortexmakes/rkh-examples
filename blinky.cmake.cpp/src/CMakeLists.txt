add_executable(${PROJECT_NAME} 
               main.cpp 
               blinky.cpp 
               bsp_blinky.cpp 
               blinky.h 
               rkhcfg.h
               signal.h
               event.h
               priority.h
               bsp.h)
target_link_libraries(${PROJECT_NAME} PRIVATE rkh)
