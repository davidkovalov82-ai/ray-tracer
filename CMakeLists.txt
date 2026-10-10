cmake_minimum_required(VERSION 3.16)
project(ray_tracer LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Общий код для программы и тестов
add_library(ray_tracer_core
    vec3.cpp
    ray.cpp
    camera.cpp
    ppm.cpp
)

target_include_directories(ray_tracer_core
    PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}"
)

add_executable(ray_tracer main.cpp)
target_link_libraries(ray_tracer PRIVATE ray_tracer_core)

# Включает тестирование и добавляет настройку BUILD_TESTING
include(CTest)

if(BUILD_TESTING)
    # Находим C++ тесты
    file(GLOB TEST_SOURCES CONFIGURE_DEPENDS
        "${CMAKE_CURRENT_SOURCE_DIR}/tests/test_*.cpp"
    )

    foreach(TEST_SOURCE IN LISTS TEST_SOURCES)
        # Например: test_camera.cpp → test_camera
        get_filename_component(TEST_NAME "${TEST_SOURCE}" NAME_WE)

        add_executable("${TEST_NAME}" "${TEST_SOURCE}")
        target_link_libraries("${TEST_NAME}" PRIVATE ray_tracer_core)

        add_test(NAME "${TEST_NAME}" COMMAND "${TEST_NAME}")
    endforeach()

    # Проверка изображения через Python
    find_package(Python3 REQUIRED COMPONENTS Interpreter)

    add_test(
        NAME stage2_image
        COMMAND "${Python3_EXECUTABLE}"
            "${CMAKE_CURRENT_SOURCE_DIR}/tests/test_stage2.py"
            --binary "$<TARGET_FILE:ray_tracer>"
    )
endif()