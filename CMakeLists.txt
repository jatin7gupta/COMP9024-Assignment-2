cmake_minimum_required(VERSION 3.14)
project(test C)

set(CMAKE_C_STANDARD 99)

add_executable(test owl2.c Quack.h Quack.c Graph.h GraphAM.c)