add_executable(${PROJECT_NAME} 
               main.c 
               blinky.c 
               blinky.h 
               rkhcfg.h
               bsp.h)
target_link_libraries(${PROJECT_NAME} PRIVATE rkh)
