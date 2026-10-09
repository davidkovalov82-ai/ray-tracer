cmake_minimum_required(VERSION 3.16)
project(ray_tracer LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_executable(ray_tracer main.cpp
        ppm.cpp
        ppm.h
        color.h
        vec3.cpp
        camera.cpp)