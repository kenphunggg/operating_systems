cmake_minimum_required(VERSION 3.10)

project(MyProject C)

include_directories(${PROJECT_SOURCE_DIR}/include)

file(GLOB SHARED_SOURCE
    ${PROJECT_SOURCE_DIR}/src/*.c)

add_executable(main ${SHARED_SOURCE} main.c)

