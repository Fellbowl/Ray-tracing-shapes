cmake_minimum_required(VERSION 3.27)
project(Proyecto1 C)

set(CMAKE_C_STANDARD 11)

add_executable(Proyecto1 main.c
        Libraries/TRA.c
        Libraries/TRA.h
        Libraries/VEC.c
        Libraries/VEC.h
        Libraries/LEC.c
        Libraries/LEC.h)
