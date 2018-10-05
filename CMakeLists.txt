cmake_minimum_required(VERSION 3.10)
project(snc C)

set(CMAKE_C_STANDARD 11)

add_executable(snc
        cmake-build-debug/CMakeCache.txt
        CMakeLists.txt
        snc.c)
