cmake_minimum_required(VERSION 3.16.3)
project(Progetto_c C)

add_executable(Progetto_c main.c)
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Werror -O2 -g3")

