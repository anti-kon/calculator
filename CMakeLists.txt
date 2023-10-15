cmake_minimum_required(VERSION 3.22)
project(calculator)

set(CMAKE_CXX_STANDARD 20)

add_executable(calculator main.cpp calculator.h calculator.cpp Operator.h)
