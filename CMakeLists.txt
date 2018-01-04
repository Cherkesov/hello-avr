cmake_minimum_required(VERSION 3.6)
project(hello_avr)

set(CMAKE_C_STANDARD 99)

set(SOURCE_FILES main.c)
add_executable(hello_avr ${SOURCE_FILES})


#add_executable(mark-1 mark-1/main.c)