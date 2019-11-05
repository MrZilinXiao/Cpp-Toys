cmake_minimum_required(VERSION 3.10)
project(Cpp-Toys)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_C_STANDARD 99)

add_executable(Cpp-Toys TextEditor/texteditor.h TextEditor/editor_main.cpp)