cmake_minimum_required(VERSION 3.20 FATAL_ERROR)

project(cpp-single-linked-list LANGUAGES C CXX)

add_executable(${PROJECT_NAME} single-linked-list/single-linked-list.h single-linked-list/main.cpp)