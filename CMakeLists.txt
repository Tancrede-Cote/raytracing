cmake_minimum_required(VERSION 3.5)
project(raytracing)
cmake_policy(SET CMP0072 NEW)
find_package(OpenGL REQUIRED)

add_executable(
    raytracing
    lib/glad/src/glad.c
    src/main.cpp
    src/app.cpp
    src/math.cpp
    src/init.cpp

)

target_include_directories(raytracing PRIVATE include)

target_link_libraries(raytracing glfw OpenGL::GL)