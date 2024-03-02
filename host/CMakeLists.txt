cmake_minimum_required(VERSION 3.27)
project(host)

set(CMAKE_CXX_STANDARD 23)

add_executable(host main.cpp
        json.h
        libexecstream/exec-stream.cpp)
