cmake_minimum_required(VERSION 3.27)
project(tash C)

set(CMAKE_C_STANDARD 11)

add_executable(tash main.c
        tash.c
        tash.h)
