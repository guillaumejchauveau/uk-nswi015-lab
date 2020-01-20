cmake_minimum_required(VERSION 3.10)
project(UNIX_Lab C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wextra")

add_subdirectory(src)
