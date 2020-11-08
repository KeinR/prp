cmake_minimum_required(VERSION 3.17)
project(here VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED true)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -g")

file(COPY slides DESTINATION "${CMAKE_BINARY_DIR}")
file(GLOB_RECURSE sources src/*.cpp)

add_executable(here ${sources})

