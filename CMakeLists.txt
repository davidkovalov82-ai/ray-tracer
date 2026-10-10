cmake_minimum_required(VERSION 3.16)
project(ray_tracer LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_executable(ray_tracer
    main.cpp
    ppm.cpp
    ppm.h
    color.h
    vec3.cpp
    camera.cpp
)

# Включаем тестирование
enable_testing()

add_executable(test_vec3
    tests/test_vec3.cpp
    vec3.cpp
)

add_executable(test_ray
    tests/test_ray.cpp
    ray.cpp
    vec3.cpp
)

add_test(NAME vec3_tests COMMAND test_vec3)
add_test(NAME ray_tests COMMAND test_ray)