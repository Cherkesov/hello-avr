cmake_minimum_required(VERSION 3.6)
project(hello_avr)

set(CMAKE_C_STANDARD 99)

set(SOURCE_FILES main.c)
add_executable(avr_examples ${SOURCE_FILES})


#add_executable(mark-1 mark-1/main.c)